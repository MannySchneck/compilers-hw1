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

Writable_Reg::Writable_Reg(std::string name) :
        Reg(name) {}


void Writable_Reg::translate(std::ostream& out) const{
        Reg::translate(out);
}

void Reg::translate(std::ostream& out) const{
        out << "%" << name;
}

Binop::Binop(Binop_Op op, std::unique_ptr<Writable> lhs, std::unique_ptr<Binop_Rhs> rhs):
        lhs(std::move(lhs)),
        op(op),
        rhs(std::move(rhs)) {}

void Binop::translate(std::ostream& out) const{
        switch(op){
        case (Binop_Op::store):
                out << "movq ";
                break;
        case (Binop_Op::add_Assign):
                break;
        case (Binop_Op::sub_Assign):
                break;
        case (Binop_Op::mult_Assign):
                break;
        case (Binop_Op::bit_And_Assign):
                break;
        case (Binop_Op::right_Shift):
                break;
        case (Binop_Op::left_Shift):
                break;
        default:
                throw new std::logic_error("Can't happen");
        }
        rhs->translate(out);
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
