#include "L1.h"
#include "L1_consts.h"
#include "catch.hpp"
#include <regex>
#include <exception>

using namespace L1;

L1::L1_Label::L1_Label() = default;
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


void Memory_Ref::translate(std::ostream& out) const{
        out << offset << "(";
        base.translate(out);
        out << ")";
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

void Comparison_Store::translate(std::ostream& out) const{
        int result;
        Cmp_Case arg_case;
        Integer_Literal* ilhs;
        Integer_Literal* irhs;
        Reg* rlhs;
        Reg* rrhs;

        if((ilhs = dynamic_cast<Integer_Literal*>(lhs.get())) &&
           (irhs = dynamic_cast<Integer_Literal*>(rhs.get()))){
                arg_case = Cmp_Case::both_Int;
        }
        // need to flip
        else if((ilhs = dynamic_cast<Integer_Literal*>(lhs.get()))
                && (rrhs = dynamic_cast<Reg*>(rhs.get()))){
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
                throw new std::logic_error("Fuck you");
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
                throw new std::logic_error("Can't happen");
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
                throw new std::logic_error("Can't happen");

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
}
