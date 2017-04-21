#include<L2/AST/shop.h>
#include<L2/AST/int_literal.h>
#include<L2/AST/regs.h>

using namespace L2;

Shop::Shop(Shop_Op op, compiler_ptr<Writable_Reg> lhs, compiler_ptr<Value_Source> rhs):
        op(op),
        lhs(std::move(lhs)),
        rhs(std::move(rhs)){}

void Shop::dump(std::ostream &out) const{
        out << "(";
        lhs->dump(out);
        switch(op){
        case(Shop_Op::left_Shift):
                out << " <<= ";
                break;
        case(Shop_Op::right_Shift):
                out << " >>= ";
                break;
        }
        rhs->dump(out);
        out << ")";
}
