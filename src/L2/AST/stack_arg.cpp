#include <L2/AST/stack_arg.h>

using namespace L2;

Stack_Arg::Stack_Arg(int64_t offset) :
        offset(offset){}

void Stack_Arg::dump(std::ostream &out) const{
        out << "(stack-arg ";
        out << offset;
        out << ")";
}

void Stack_Arg::accept(AST_Item_Visitor &v){
        v.visit(this);
}
