#include <L2/AST/function.h>
#include <L2/AST/goto.h>
#include <L2/AST/cond_jump.h>
#include <L2/AST/shop.h>
#include <L2/AST/var.h>
#include <vector>
#include <iterator>
#include <numeric>
#include <iostream>
#include "L2/reg_allocation/interference_graph.h"
#include <prettyprint.hpp>

using namespace L2;


Function::Function(L2_Target_Label name, int64_t args, int64_t locals) :
        name(name),
        arguments(args),
        locals(locals){}

int64_t Function::stack_args() const{
        return arguments < 6 ? 0 : arguments - 6;
}

int64_t Function::stack_shift() const{
        return 8 * (locals  + stack_args());
}

void Function::expand_stack(std::ostream&out) const{
        int _stack_shift = stack_shift();
        if(_stack_shift){
                out << "\n\tsubq $" << _stack_shift << ", %rsp";
        }
}

void Function::shrink_stack(std::ostream&out) const{
        int _stack_shift = stack_shift();
        if(_stack_shift){
                out << "\n\taddq $" << _stack_shift << ", %rsp";
        }
}


void Function::dump(std::ostream &out) const{
        out << "(";
        name.dump(out);
        out << std::endl;
        out << arguments << " " << locals << std::endl;

        for(auto& inst : instructions){
                inst->dump(out);
                out << std::endl;
        }

        out << ")" << std::endl;
}



std::vector<Inst_Posn> Function::find_successors(Inst_Posn pos){

        return (*pos)->find_successors(pos, instructions);
}

template <typename T>
class TD;

// XXX: mutation semantics. BAD
void Function::populate_liveness_sets(){

        bool dirty{false};
        do {
                dirty = false;
                for(auto inst_it = instructions.end() - 1;
                    inst_it != instructions.begin() - 1;
                    inst_it--){
                        dirty = dirty || (*inst_it)->one_iteration_calc_io_sets(inst_it,
                                                                                instructions);
                }
        } while(dirty);
}

liveness_sets_t Function::make_liveness_sets(){
        std::vector<io_set_t> in_sets;
        std::vector<io_set_t> out_sets;

        for(auto inst : instructions){
                in_sets.push_back(inst->in);
                out_sets.push_back(inst->out);
        }

        return liveness_sets_t{in_sets, out_sets};
}


// TODO: filter on reg vs var
// TODO: XXX FIXME XXX 
Interference_Graph Function::
make_interference_graph(){
        // This isn't shitty at all.

        Interference_Graph interference_graph;

        populate_liveness_sets();

        for(auto inst : instructions){
                // connect each pair of IDs that appear in the same in or
                // out set
                std::cout << "DUMPING IO SETS" << std::endl;
                interference_graph.draw_intra_set_edges(inst->in);
                std::cout << inst->in << std::endl;
                interference_graph.draw_intra_set_edges(inst->out);
                std::cout << inst->out << std::endl;
                std::cout << "/DUMP" << std::endl;


                // connect the kill set to the out set (unless doing a store)
                auto p = dynamic_cast<Binop*>(inst.get());
                if(p && p->op != Binop_Op::store){ // =(
                        for(auto id : inst->kill()){
                                for(auto id_prime : inst->out){
                                        if(id != id_prime &&
                                           !Lang_Constants::regs_set.count(id)){
                                                interference_graph.add_edge(id, id_prime);
                                        }
                                }
                        }
                }

                // handle architectural requirements
                // c++ is terrible. How do you concatenate compile time
                // constant arrays?
                // pre: Assumes correct l2 program. Never given a reg other than rcx
                auto shop_p = dynamic_cast<Shop*>(inst.get());
                L2_ID* id_p;
                if(shop_p &&
                   (id_p = dynamic_cast<Var*>(shop_p->rhs.get()))){ // =( so much for design...
                        for(auto reg :  Lang_Constants::regs_vector){
                                if(reg != "rcx"){
                                        interference_graph.add_edge(id_p->get_name(), reg);
                                }
                        }
                }
        }
        // If I was a real programmer I'd refactor these for loops into
        // functions

        return interference_graph;
}

void spill_these(std::vector<compiler_ptr<IG_Node>>){
        ;
}

compiler_ptr<Function> Function::allocate_registers(){
        populate_liveness_sets();

        bool allocated{false};

        Interference_Graph interference_graph;
        do{
                interference_graph = make_interference_graph();

                std::vector<compiler_ptr<IG_Node>>
                        spills = interference_graph.attempt_coloring();

                if(spills.size() == 0){ // nothing to spill, we're done
                        allocated = true;
                } else {
                        // spill and go again
                        allocated = true; //XXX WIP
                        spill_these(spills);
                }

        } while(!allocated);

        std::cout << "DUMPING COLORED IG" << std::endl;
        std::cout << interference_graph << std::endl;
        std::cout << "DUMPING IG" << std::endl;

        auto regs_only_new_f  = std::make_shared<Function>();

        regs_only_new_f->name = name;

        for(auto instr : instructions){
                regs_only_new_f->instructions.
                        push_back(instr->
                                  replace_vars(interference_graph.
                                               get_reg_map()));
        }

        return regs_only_new_f;
}

void Function::spill_these(std::vector<compiler_ptr<IG_Node>> spills){
        ;
}
