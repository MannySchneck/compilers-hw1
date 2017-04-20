#include <L2/AST/return.h>

using namespace L2;


Return::Return(int64_t stack_shift):
        stack_shift(stack_shift){}


void Return::translate(std::ostream &out) const{
        if(stack_shift){
                out << "\n\taddq $" << stack_shift << ", %rsp";
        }
        out << "\n\tretq";
}


void Return::dump(std::ostream &out) const{
        out << "ACK!";
}
