#include <L1/AST/memory_ref.h>

using namespace L1;

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


