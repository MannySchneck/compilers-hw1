#include <L2/AST/int_literal.h>

using namespace L2;

Integer_Literal::Integer_Literal(std::string val){
        value = std::stoll(val);
}

void Integer_Literal::dump(std::ostream &out) const{
        out << value;
}

void Integer_Literal::accept(AST_Item_Visitor &v){
        v.visit(this);
}
