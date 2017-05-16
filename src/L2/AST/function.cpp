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
#include <sstream>
#include <algorithm>
#include <random>
#include <memory>

#include <cassert>

#include <catch.hpp>

using namespace L2;


Function::Function() :
        arguments(0),
        locals(0),
        prefix_counter(0),
        prefix(boost::none){}

Function::Function(L2_Target_Label name, int64_t args, int64_t locals) :
        name(name),
        arguments(args),
        locals(locals),
        prefix_counter(0),
        prefix(boost::none){}

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

                interference_graph.draw_intra_set_edges(inst->in);
                interference_graph.draw_intra_set_edges(inst->out);

                // connect each pair of IDs that appear in the same in or
                // out set
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

        return interference_graph;
}

using str_to_str_umap = std::unordered_map<std::string, std::string>;
using i_ptr_vec = std::vector<compiler_ptr<Instruction>>;
void Function::insert_spill_accesses(i_ptr_vec::iterator pos,
                                     const std::string &id_to_spill,
                                     const str_to_str_umap &spill_map,
                                     i_ptr_vec &new_instrs){

        assert(pos != instructions.end());

        //XXX probably a bug here...
        locals += 1;
        int64_t offset = locals * 8;

        compiler_ptr<AST_Item> spill_var{new Var{spill_map.at(id_to_spill)}};
        compiler_ptr<AST_Item> spill_mem_ref{new Memory_Ref{
                        compiler_ptr<L2_ID>{new Reg{"rsp"}},
                                offset}};

        // This is terrible
        auto spill_var_read = std::dynamic_pointer_cast<Binop_Lhs>(spill_var);
        auto spill_var_write = std::dynamic_pointer_cast<Binop_Rhs>(spill_var);
        auto spill_mem_ref_read = std::dynamic_pointer_cast<Binop_Lhs>(spill_mem_ref);
        auto spill_mem_ref_write = std::dynamic_pointer_cast<Binop_Rhs>(spill_mem_ref);


        compiler_ptr<Instruction> spill_read{new Binop{
                        Binop_Op::store,
                                spill_var_read,
                                spill_mem_ref_write}};
        compiler_ptr<Instruction> spill_write{new Binop{
                        Binop_Op::store,
                                spill_mem_ref_read,
                                spill_var_write}};

        new_instrs.push_back(spill_read);
        new_instrs.push_back((*pos)->replace_vars(spill_map));
        new_instrs.push_back(spill_write);
}


// This is very inefficient, but it lets me reuse the var-replacement
// machinery that I've built, so I'm doing this. Sorry.
std::vector<compiler_ptr<Instruction>>
Function::spill_these(std::vector<compiler_ptr<IG_Node>> spills){
        std::string spill_prefix = get_prefix().append(std::to_string(prefix_counter));

        std::unordered_map<std::string, std::string> spill_map;

        // build the spill map
        for(auto node_ptr : spills){
                auto node_name = node_ptr->get_name();

                // If we're spilling registers, something has gone very wrong
                assert(!Lang_Constants::regs_set.count(node_name));

                auto node_spill_name = spill_prefix;
                node_spill_name.append(node_name);
                spill_map[node_name] = node_spill_name;
        }

        // add spill writes/reads for instructions that use spilled vars

        std::vector<compiler_ptr<Instruction>> new_instrs;
        for(auto instr = instructions.begin();
            instr != instructions.end();
            instr++){
                auto i_ptr  = *instr;
                Get_Ids_Visitor v{};

                i_ptr->accept(v);

                bool spilled{false};
                for(auto id : v.result){
                        if(spill_map.count(id)){
                                spilled = true;
                                insert_spill_accesses(instr, id, spill_map, new_instrs);
                        }
                }

                if(!spilled){
                        new_instrs.push_back(i_ptr);
                }
        }

        prefix_counter += 1;

        return new_instrs;
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
                        allocated = false;
                        instructions = spill_these(spills);
                }

        } while(!allocated);


        Get_Ids_Visitor v;
        for(auto inst : instructions){
                inst->accept(v);
        }

        for(auto id : v.result){
                if(id != "rsp"){
                        assert((interference_graph.nodes_set.count(id) &&
                                interference_graph.nodes_set.at(id)->color));
                }
        }

        auto regs_only_new_f  = std::make_shared<Function>();
        regs_only_new_f->name = name;

        for(auto instr : instructions){
                auto new_instr =  instr->
                        replace_vars(interference_graph.
                                     get_reg_map());

                regs_only_new_f->instructions. push_back(new_instr);
        }

        return regs_only_new_f;
}

std::string Function::find_prefix(){
        std::vector<std::string> symbols;
        for(auto instr : instructions){
                Get_Ids_Visitor v{};
                instr->accept(v);
                symbols.insert(symbols.end(),
                               v.result.begin(),
                               v.result.end());
        }

        std::sort(symbols.begin(), symbols.end());

        auto z_symbol =
                std::find_if(symbols.begin(),
                             symbols.end(),
                             [](const std::string& s){
                                     return s[0] == 'z';
                             });

        if(z_symbol == symbols.end()){
                return std::string{'z'};
        }
        else{
                char next_char = 'a';
                std::string the_prefix{"za"};
                for(; z_symbol != symbols.end(); z_symbol++){
                        auto symbol = *z_symbol;

                        bool found_prefix{false};
                        // the_prefix can't collide with anything previous
                        // since if collision was possible, it would have
                        // sorted after the string we're checking

                        // This is so fucking terrible
                        while(!found_prefix){

                                if(next_char == '{'){
                                        next_char = 'a';
                                        the_prefix.push_back(next_char);
                                }

                                if(the_prefix == symbol.substr(0, the_prefix.length())){
                                        char prefix_add =
                                                symbol[the_prefix.length() - 1] + 1;
                                        the_prefix[the_prefix.length() - 1] = prefix_add;
                                        next_char = prefix_add + 1;
                                }
                                else{
                                        found_prefix = true;
                                }
                        }
                }
                return the_prefix;
        }
}



std::string Function::get_prefix(){
        if(prefix){
                return *prefix;
        }

        else{
                prefix = find_prefix();
        }

        return *prefix;
}


char rando_chardrissian(){
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> upcase_ascii(65, 90);
        const static int to_lowcase = 32;

        const static int odd_mask = 0x1;
        int uncased_char_ = upcase_ascii(gen);
        int char_ = upcase_ascii(gen) & 0x1 ? uncased_char_ : uncased_char_ + to_lowcase;

        return static_cast<char>(char_);
}
