#include <L1/AST/lea.h>

using namespace L1;

LEA::LEA(Writable_Reg target, Writable_Reg base, Writable_Reg offset, int64_t mult) :
        target(target),
        base(base),
        offset(offset),
        mult(mult)
{
        if((mult % 2) || mult > 8){
                throw std::invalid_argument("mult must be even, and 8 or less");
        }
}

void LEA::translate(std::ostream& out) const{
        out << "lea (";
        base.translate(out);
        out << ", ";
        offset.translate(out);
        out << ", " << mult;
        out << "), ";
        target.translate(out);
}
