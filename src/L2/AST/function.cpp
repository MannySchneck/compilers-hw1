#include <L2/AST/function.h>
#include <L2/AST/goto.h>
#include <L2/AST/cond_jump.h>
#include <vector>
#include <iterator>
#include <numeric>

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



