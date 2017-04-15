#include "L1.h"
#include "L1_consts.h"
#include "catch.hpp"
#include <regex>
#include <exception>

using namespace L1;

L1_Label::L1_Label() {};

L1::L1_Label::L1_Label(std::string label) :
        labelName(label){};

void L1::L1_Label::translate(std::ostream &out) const {
        out << "_" << labelName.substr(1, labelName.size());
}

Integer_Literal::Integer_Literal(std::string val){
        value = std::stoll(val);
}

void Integer_Literal::translate(std::ostream &out) const{
        out << "$" << value;
}

Reg::Reg(std::string name) :
        name(name){}

Reg Reg::get_eight_bit() const{
        return Reg(eight_bit_name.at(name));
}

const std::unordered_map<std::string, std::string> Reg::eight_bit_name =
{
        {"r8", "r8b"},
        {"r9", "r9b"},
        {"r10", "r10b"},
        {"r11", "r11b"},
        {"r12", "r12b"},
        {"r13", "r13b"},
        {"r14", "r14b"},
        {"r15", "r15b"},
        {"rax", "al"},
        {"rbx", "bl"},
        {"rcx", "cl"},
        {"rdx", "dl"},
        {"rdi", "dil"},
        {"rsi", "sil"},
        {"rbp", "bpl"}
};


Writable_Reg::Writable_Reg(std::string name) :
        Reg(name) {}


void Writable_Reg::translate(std::ostream& out) const{
        Reg::translate(out);
}

void Reg::translate(std::ostream& out) const{
        out << "%" << name;
}


Memory_Ref::Memory_Ref(Reg base, int64_t offset) :
        base(base),
        offset(offset)
{
        if(offset % 8 != 0){
                throw std::invalid_argument("memory ref must be multiple of 8");
        }
};

void Memory_Ref::translate(std::ostream& out) const{
        out << offset << "(";
        base.translate(out);
        out << ")";
}


LEA::LEA(Writable_Reg target, Writable_Reg base, Writable_Reg offset, int64_t mult) :
        target(target),
        base(base),
        offset(offset),
        mult(mult)
{
        if((mult % 2) || mult > 8){
                throw std::invalid_argument("mult must be even, and 8 or less");
        }
}

void LEA::translate(std::ostream& out) const{
        out << "lea (";
        base.translate(out);
        out << ", ";
        offset.translate(out);
        out << ", " << mult;
        out << "), ";
        target.translate(out);
}

Comparison_Store::Comparison_Store(Cmp_Op op,
                                   std::unique_ptr<Value_Source> lhs,
                                   std::unique_ptr<Value_Source> rhs,
                                   Writable_Reg target) :
        op(op),
        target(target),
        lhs(std::move(lhs)),
        rhs(std::move(rhs)){}

enum class Cmp_Case{
        both_Int,
                flip,
                };

void output_comparison(Translatable* lhs,
                       Translatable* rhs,
                       std::ostream& out){
        out << "cmpq ";
        rhs->translate(out);
        out << ", ";
        lhs->translate(out);
}

void output_comparison_store(Translatable* lhs,
                             Translatable* rhs,
                             Writable_Reg target,
                             std::string cmp_set,
                             std::ostream& out){
        output_comparison(lhs, rhs, out);
        out << "\n\t" << cmp_set << " ";
        target.get_eight_bit().translate(out);
        out << "\n\tmovzbq ";
        target.get_eight_bit().translate(out);
        out << ", ";
        target.translate(out);

}

void output_mov(Writable_Reg target, int val, std::ostream& out){
        out << "movq " << "$" << val << ", ";
        target.translate(out);
}

Cond_Jump::Cond_Jump(Cmp_Op cmp,
                     std::unique_ptr<Value_Source> Cmp_Lhs,
                     std::unique_ptr<Value_Source> Cmp_Rhs,
                     L1_Label true_target,
                     L1_Label false_target) :
        cmp(cmp),
        Cmp_Lhs(std::move(Cmp_Lhs)),
        Cmp_Rhs(std::move(Cmp_Rhs)),
        true_target(true_target),
        false_target(false_target){}


void output_const_cjump(int result, const L1_Label& t_target, const L1_Label& f_target, std::ostream& out){
        if(result){
                out << "jmp ";
                t_target.translate(out);
        } else{
                out << "jmp ";
                f_target.translate(out);
        }
}

void gen_cjump_text(Translatable* lhs,
                    Translatable* rhs,
                    L1_Label t_t,
                    L1_Label f_t,
                    std::string jmp,
                    std::ostream& out){
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

Goto::Goto(std::string lab) :
        target(new L1_Label{lab}){};

Goto::Goto(std::unique_ptr<L1_Label> lab) :
        target(std::move(lab)){}

void Goto::translate(std::ostream& out) const{
        out << "jmp ";
        target->translate(out);
}


void Comparison_Store::translate(std::ostream& out) const{
        int result;
        Cmp_Case arg_case;
        Integer_Literal* ilhs;
        Integer_Literal* irhs;

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
                out << "dec";
                target->translate(out);
                break;
        }
}

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
        rhs->translate(out);
        out << ", ";
        lhs->translate(out);
}

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

Call::Call(std::unique_ptr<Callable> fun, int64_t numargs) :
        fun(std::move(fun)),
        numargs(numargs){}

void Call::translate(std::ostream& out) const{
        out << "subq $8, %rsp";
        out << "\n\tjmp ";

        if(dynamic_cast<Writable_Reg*>(fun.get())){
                out << "*";
                fun->translate(out);
        } else {
                fun->translate(out);
        }
}

Runtime_Call::Runtime_Call(Runtime_Fun fun) :
        fun(fun){}

void Runtime_Call::translate(std::ostream& out) const{
        out << "call ";
        switch(fun){
        case (Runtime_Fun::print):
                out << "print";
                break;
        case (Runtime_Fun::alloc):
                out << "alloc";
                break;
        case (Runtime_Fun::array_Error):
                out << "array_Error";
                break;

        }
}


Function::Function(L1_Label name, int64_t args, int64_t locals) :
        name(name),
        arguments(args),
        locals(locals){}

int64_t Function::stack_args() const{
        return arguments < 6 ? 0 : arguments - 6;
}

void Function::translate(std::ostream &out) const{
        name.translate(out);

        out << "\n\t";
        for(auto it = instructions.cbegin(); it != instructions.cend(); it++){
                (*it)->translate(out);
                out << "\n\t";
        }

        out << "addq $" << 8 * (locals  + stack_args()) << ", %rsp";
        out << "\n\t" << "retq";
}

L1::Program::Program(std::string label) :
        entryPointLabel(label){}

void L1::Program::translate(std::ostream &out) const {
        out << prog_prefix;

        for(auto it = callee_saves().cbegin(); it != callee_saves().cend(); ++it){
                out << "\t" << "pushq" << "\t" << "%" << *it << "\n";
        }

        out << "\t\n";
        out << "call ";
        entryPointLabel.translate(out);
        out << "\t\n\t\n";

        for(auto it = callee_saves().crbegin(); it != callee_saves().crend(); ++it){
                out << "\t" << "popq" << "\t" << "%" << *it << "\n";
        }

        out << "\tretq";

        for(auto& fun : functions){
                out << "\n";
                fun->translate(out);
        }
}
