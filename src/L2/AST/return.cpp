#include <L2/AST/return.h>

using namespace L2;


Return::Return(int64_t stack_shift):
        stack_shift(stack_shift){}



void Return::dump(std::ostream &out) const{
        out << "(return)";
}
