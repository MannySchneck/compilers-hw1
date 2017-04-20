#include <L2/AST/cond_jump.h>
#include <L2/AST/cmp_ops.h>
#include <L2/AST/int_literal.h>
#include <L2/AST/regs.h>

using namespace L2;

Cond_Jump::Cond_Jump(Cmp_Op cmp,
                     std::unique_ptr<Value_Source> Cmp_Lhs,
                     std::unique_ptr<Value_Source> Cmp_Rhs,
                     L2_Label true_target,
                     L2_Label false_target) :
        cmp(cmp),
        Cmp_Lhs(std::move(Cmp_Lhs)),
        Cmp_Rhs(std::move(Cmp_Rhs)),
        true_target(true_target),
        false_target(false_target){}


void Cond_Jump::output_const_cjump(int result, const L2_Label& t_target, const L2_Label& f_target, std::ostream& out) const {
        if(result){
                out << "jmp ";
                t_target.translate(out);
        } else{
                out << "jmp ";
                f_target.translate(out);
        }
}



void Cond_Jump::gen_cjump_text(Translatable* lhs,
                    Translatable* rhs,
                    L2_Label t_t,
                    L2_Label f_t,
                    std::string jmp,
                    std::ostream& out) const{
        output_comparison(lhs, rhs, out);
        out << "\n\t" << jmp << " ";
        t_t.translate(out);
        out << "\n\t" << "jmp ";
        f_t.translate(out);
}

void Cond_Jump::translate(std::ostream& out) const{
        int result;
        Cmp_Case arg_case;
        Integer_Literal* ilhs;
        Integer_Literal* irhs;

        if((ilhs = dynamic_cast<Integer_Literal*>(Cmp_Lhs.get())) &&
           (irhs = dynamic_cast<Integer_Literal*>(Cmp_Rhs.get()))){
                arg_case = Cmp_Case::both_Int;
        }
        // need to flip
        else if(dynamic_cast<Integer_Literal*>(Cmp_Lhs.get())
                && dynamic_cast<Reg*>(Cmp_Rhs.get())){
                arg_case = Cmp_Case::flip;
        }

        switch(cmp){
        case (Cmp_Op::equal):
                switch(arg_case){
                case (Cmp_Case::both_Int):
                        result = ilhs->value == irhs->value;
                        output_const_cjump(result, true_target, false_target, out);
                        break;
                case(Cmp_Case::flip):
                        gen_cjump_text(Cmp_Rhs.get(),
                                       Cmp_Lhs.get(),
                                       true_target,
                                       false_target,
                                       "je",
                                       out);
                        break;
                default:
                        gen_cjump_text(Cmp_Lhs.get(),
                                       Cmp_Rhs.get(),
                                       true_target,
                                       false_target,
                                       "je",
                                       out);
                        break;
                }
                break;
        case (Cmp_Op::less):
                switch(arg_case){
                case (Cmp_Case::both_Int):
                        result = ilhs->value < irhs->value;
                        output_const_cjump(result, true_target, false_target, out);
                        break;
                case(Cmp_Case::flip):
                        gen_cjump_text(Cmp_Rhs.get(),
                                       Cmp_Lhs.get(),
                                       true_target,
                                       false_target,
                                       "jg",
                                       out);
                        break;
                default:
                        gen_cjump_text(Cmp_Lhs.get(),
                                       Cmp_Rhs.get(),
                                       true_target,
                                       false_target,
                                       "jl",
                                       out);
                        break;
                }

                break;
        case (Cmp_Op::less_Equal):
                switch(arg_case){
                case (Cmp_Case::both_Int):
                        result = ilhs->value <= irhs->value;
                        output_const_cjump(result, true_target, false_target, out);
                        break;
                case(Cmp_Case::flip):
                        gen_cjump_text(Cmp_Rhs.get(),
                                       Cmp_Lhs.get(),
                                       true_target,
                                       false_target,
                                       "jge",
                                       out);
                        break;
                default:
                        gen_cjump_text(Cmp_Lhs.get(),
                                       Cmp_Rhs.get(),
                                       true_target,
                                       false_target,
                                       "jle",
                                       out);
                        break;
                }
                break;
        default:
                throw std::logic_error("Fuck you");
        }

}

void Cond_Jump::dump(std::ostream &out) const{
        out << "ACK!";
}
