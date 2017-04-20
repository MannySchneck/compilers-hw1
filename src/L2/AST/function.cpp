#include <L2/AST/function.h>

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


void Function::translate(std::ostream &out) const{
        name.translate(out);

        expand_stack(out);

        out << "\n\t";
        for(auto it = instructions.cbegin(); it != instructions.cend(); it++){
                (*it)->translate(out);
                out << "\n\t";
        }
}
void Function::dump(std::ostream &out) const{
        out << "ACK!";
}
