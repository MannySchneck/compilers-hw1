#include <L2/AST/monop.h>

using namespace L2;

Monop::Monop(Monop_Op op, std::unique_ptr<Writable_Reg> target) :
        op(op),
        target(std::move(target)){};

void Monop::translate(std::ostream& out) const{
        switch(op){
        case (Monop_Op::inc):
                out << "inc ";
                target->translate(out);
                break;
        case (Monop_Op::dec):
                out << "dec ";
                target->translate(out);
                break;
        }
}

void Monop::dump(std::ostream &out) const{
        out << "ACK!";
}
