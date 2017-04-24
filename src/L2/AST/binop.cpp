#include <L2/AST/binop.h>
#include <L2/AST/labels.h>
#include <ostream>

using namespace L2;

Binop::Binop(Binop_Op op, compiler_ptr<Binop_Lhs> lhs, compiler_ptr<Binop_Rhs> rhs):
        lhs(std::move(lhs)),
        op(op),
        rhs(std::move(rhs)) {}

void Binop::dump(std::ostream &out) const{
        out << "(";
        lhs->dump(out);

        switch(op){
        case(Binop_Op::store):
                out << " <- ";
                break;
        case(Binop_Op::add_Assign):
                out << " += ";
                break;
        case(Binop_Op::sub_Assign):
                out << " -= ";
                break;
        case(Binop_Op::mult_Assign):
                out << " *= ";
                break;
        case(Binop_Op::bit_And_Assign):
                out << " &= ";
                break;
        }

        rhs->dump(out);
        out << ")";
}
