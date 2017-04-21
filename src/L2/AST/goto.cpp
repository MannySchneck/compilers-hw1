#include <L2/AST/labels.h>
#include <vector>
#include <L2/AST/goto.h>

using namespace L2;

Goto::Goto(std::string lab) :
        target(new L2_Label{lab}){};

Goto::Goto(compiler_ptr<L2_Label> lab) :
        target(std::move(lab)){}

void Goto::dump(std::ostream &out) const{
        out << "(";
        out << "goto ";
        target->dump(out);
        out << ")";
}
