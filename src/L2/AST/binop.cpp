#include <L2/AST/binop.h>
#include <L2/AST/labels.h>
#include <ostream>

using namespace L2;

Binop::Binop(Binop_Op op, std::unique_ptr<Binop_Lhs> lhs, std::unique_ptr<Binop_Rhs> rhs):
        lhs(std::move(lhs)),
        op(op),
        rhs(std::move(rhs)) {}

void Binop::translate(std::ostream& out) const{
        switch(op){
        case (Binop_Op::store):
                out << "movq ";
                break;
        case (Binop_Op::add_Assign):
                out << "addq ";
                break;
        case (Binop_Op::sub_Assign):
                out << "subq ";
                break;
        case (Binop_Op::mult_Assign):
                out << "imulq ";
                break;
        case (Binop_Op::bit_And_Assign):
                out << "andq ";
                break;
        default:
                throw std::logic_error("Can't happen");
        }

        if(dynamic_cast<L2_Label*>(rhs.get())){
                out << "$";
        }

        rhs->translate(out);
        out << ", ";
        lhs->translate(out);
}

void Binop::dump(std::ostream &out) const{
        out << "ACK!";
}

