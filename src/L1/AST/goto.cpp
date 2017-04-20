#include <L1/AST/labels.h>
#include <vector>
#include <L1/AST/goto.h>

namespace L1{
        Goto::Goto(std::string lab) :
                target(new L1_Label{lab}){};

        Goto::Goto(std::unique_ptr<L1_Label> lab) :
                target(std::move(lab)){}

        void Goto::translate(std::ostream& out) const{
                out << "jmp ";
                target->translate(out);
        }


}
