#include<L1/AST/shop.h>
#include<L1/AST/int_literal.h>
#include<L1/AST/regs.h>

using namespace L1;

Shop::Shop(Shop_Op op, std::unique_ptr<Writable_Reg> lhs, std::unique_ptr<Value_Source> rhs):
        op(op),
        lhs(std::move(lhs)),
        rhs(std::move(rhs)){}

void Shop::translate(std::ostream& out) const{
        switch(op){
        case (Shop_Op::left_Shift):
                out << "salq ";
                break;
        case (Shop_Op::right_Shift):
                out << "sarq ";
                break;
        default:
                throw std::logic_error("Can't happen");

        }

        Reg* is_reg;
        Integer_Literal* is_int;
        if((is_reg = dynamic_cast<Reg*>(rhs.get()))){
                (is_reg->get_eight_bit()).translate(out);
        }

        if((is_int = dynamic_cast<Integer_Literal*>(rhs.get()))){
                is_int->translate(out);
        }
        out << ", ";
        lhs->translate(out);
}
