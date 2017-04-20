#include <L1/AST/int_literal.h>

using namespace L1;

Integer_Literal::Integer_Literal(std::string val){
        value = std::stoll(val);
}

void Integer_Literal::translate(std::ostream &out) const{
        out << "$" << value;
}
