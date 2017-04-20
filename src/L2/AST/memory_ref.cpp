#include <L2/AST/memory_ref.h>

using namespace L2;

Memory_Ref::Memory_Ref(Reg base, int64_t offset) :
        base(base),
        offset(offset)
{
        if(offset % 8 != 0){
                throw std::invalid_argument("memory ref must be multiple of 8");
        }
};

void Memory_Ref::translate(std::ostream& out) const{
        out << offset << "(";
        base.translate(out);
        out << ")";
}

void Memory_Ref::dump(std::ostream &out) const{
        out << "ACK!";
}
