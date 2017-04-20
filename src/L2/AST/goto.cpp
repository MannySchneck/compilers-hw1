#include <L2/AST/labels.h>
#include <vector>
#include <L2/AST/goto.h>

using namespace L2;

Goto::Goto(std::string lab) :
        target(new L2_Label{lab}){};

Goto::Goto(std::unique_ptr<L2_Label> lab) :
        target(std::move(lab)){}

void Goto::translate(std::ostream& out) const{
        out << "jmp ";
        target->translate(out);
}



void Goto::dump(std::ostream &out) const{
        out << "ACK!";
}
