#include <L1/AST/cmp_store.h>
#include <L1/AST/int_literal.h>

using namespace L1;

Comparison_Store::Comparison_Store(Cmp_Op op,
                                   std::unique_ptr<Value_Source> lhs,
                                   std::unique_ptr<Value_Source> rhs,
                                   Writable_Reg target) :
        op(op),
        target(target),
        lhs(std::move(lhs)),
        rhs(std::move(rhs)){}


void Comparison_Store::translate(std::ostream& out) const{
        int result;
        Cmp_Case arg_case = Cmp_Case::nothing;
        Integer_Literal* ilhs = nullptr;
        Integer_Literal* irhs = nullptr;

        if((ilhs = dynamic_cast<Integer_Literal*>(lhs.get())) &&
           (irhs = dynamic_cast<Integer_Literal*>(rhs.get()))){
                arg_case = Cmp_Case::both_Int;
        }
        // need to flip
        else if(dynamic_cast<Integer_Literal*>(lhs.get())
                && dynamic_cast<Reg*>(rhs.get())){
                arg_case = Cmp_Case::flip;
        }


        switch(op){
        case (Cmp_Op::equal):
                switch(arg_case){
                case (Cmp_Case::both_Int):
                        result = ilhs->value == irhs->value;
                        output_mov(target, result, out);
                        break;
                case(Cmp_Case::flip):
                        output_comparison_store(rhs.get(), lhs.get(), target, "sete", out);
                        break;
                default:
                        output_comparison_store(lhs.get(), rhs.get(), target, "sete", out);
                        break;
                }
                break;
        case (Cmp_Op::less):
                switch(arg_case){
                case (Cmp_Case::both_Int):
                        result = ilhs->value < irhs->value;
                        output_mov(target, result, out);
                        break;
                case(Cmp_Case::flip):
                        output_comparison_store(rhs.get(), lhs.get(), target, "setg", out);
                        break;
                default:
                        output_comparison_store(lhs.get(), rhs.get(), target, "setl", out);
                        break;
                }

                break;
        case (Cmp_Op::less_Equal):
                switch(arg_case){
                case (Cmp_Case::both_Int):
                        result = ilhs->value <= irhs->value;
                        output_mov(target, result, out);
                        break;
                case(Cmp_Case::flip):
                        output_comparison_store(rhs.get(), lhs.get(), target, "setge", out);
                        break;
                default:
                        output_comparison_store(lhs.get(), rhs.get(), target, "setle", out);
                        break;
                }
                break;
        default:
                throw std::logic_error("Fuck you");
        }
}

void Comparison_Store::output_comparison_store(Translatable* lhs,
                                               Translatable* rhs,
                                               Writable_Reg target,
                                               std::string cmp_set,
                                               std::ostream& out) const{
        output_comparison(lhs, rhs, out);
        out << "\n\t" << cmp_set << " ";
        target.get_eight_bit().translate(out);
        out << "\n\tmovzbq ";
        target.get_eight_bit().translate(out);
        out << ", ";
        target.translate(out);
}

void Comparison_Store::output_mov(Writable_Reg target, int val, std::ostream& out) const {
        out << "movq " << "$" << val << ", ";
        target.translate(out);
}

