#include <L2/AST/function.h>
#include <L2/AST/goto.h>
#include <L2/AST/cond_jump.h>
#include <vector>
#include <iterator>
#include <numeric>
#include <L2/reg_allocation/interference_graph.h>

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

bool not_in_reg_set(std::string id){
        return static_cast<bool>(Lang_Constants::regs_set.count(id) == 0);
}

adjacency_set_t draw_intra_set_edges(adjacency_set_t &adj, const io_set_t &st){
        for(auto id : st){
                IG_Node node (id);
                for(auto id_prime : st){
                        if(id != id_prime && not_in_reg_set(id))
                                adj[node].insert(IG_Node(id_prime));
                }
        }

        return adj;
}

// TODO: filter on reg vs var
// pre: liveness sets are populated!
// pre: inter-reg connections have been added
adjacency_set_t Function::
populate_interference_graph(adjacency_set_t adjacency_set){
        // This isn't shitty at all.
        for(auto inst : instructions){
                // connect each pair of IDs that appear in the same in or
                // out set

                draw_intra_set_edges(adjacency_set, inst->in);

                draw_intra_set_edges(adjacency_set, inst->out);

                // connect the kill set to the out set (unless doing a store)
                auto p = dynamic_cast<Binop*>(inst.get());
                if(p && p->op != Binop_Op::store){ // =(
                        for(auto id : inst->kill()){
                                IG_Node node(id);
                                for(auto id_prime : inst->out){
                                        if(id != id_prime && not_in_reg_set(id))
                                                adjacency_set[node].insert(IG_Node(id_prime));
                                }
                        }
                }

                // handle architectural requirements
                // c++ is terrible. How do you concatenate compile time
                // constant arrays?
                // pre Assumes correct l2 program. Never given a reg other than rcx
                auto shop_p = dynamic_cast<Shop*>(inst.get());
                L2_ID* id_p;
                if(shop_p &&
                   (id_p = dynamic_cast<Var*>(shop_p->rhs.get()))){ // =( so much for design...
                        IG_Node node{id_p->get_name()};
                        for(auto reg :  Lang_Constants::regs_vector){
                                if(reg != "rcx"){
                                        adjacency_set[node].insert(IG_Node{reg});
                                }
                        }
                }
        }
        // If I was a real programmer I'd refactor these for loops into
        // functions

        return adjacency_set;
}
