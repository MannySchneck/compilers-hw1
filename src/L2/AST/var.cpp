#include <L2/AST/var.h>

using namespace L2;

Var::Var(std::string name) :
        L2_ID(name){}

void Var::dump(std::ostream &out) const{
        out << name;
}

//;;;
