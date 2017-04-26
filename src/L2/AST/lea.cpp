#include <L2/AST/lea.h>
#include <unordered_set>
#include <string>

using namespace L2;

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

void LEA::dump(std::ostream &out) const{
        out << "lea!";
}

io_set_t LEA::gen() const{
        io_set_t gen_st;

        return gen_st;
}

io_set_t LEA::kill() const{
        io_set_t kill_st;

        return kill_st;
}
