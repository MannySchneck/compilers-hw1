#include <L2/AST/binop.h>
#include <L2/AST/labels.h>
#include <ostream>
#include <L2/AST/regs.h>
#include <exception>
#include <iostream>

using namespace L2;

Binop::Binop(Binop_Op op, compiler_ptr<Binop_Lhs> lhs, compiler_ptr<Binop_Rhs> rhs):
        lhs(std::move(lhs)),
        op(op),
        rhs(std::move(rhs)) {}

std::string Binop::stringify_binop_op(Binop_Op op) const{

        switch(op){
        case(Binop_Op::store):
                return "<-";
                break;
        case(Binop_Op::add_Assign):
                return "+=";
                break;
        case(Binop_Op::sub_Assign):
                return "-=";
                break;
        case(Binop_Op::mult_Assign):
                return "*=";
                break;
        case(Binop_Op::bit_And_Assign):
                return "&=";
                break;
        default:
                throw std::logic_error("Invalid binop op");
        }
}

void Binop::dump(std::ostream &out) const{
        out << "(";
        lhs->dump(out);

        out << " " << stringify_binop_op(op);

        rhs->dump(out);
        out << ")";
}

io_set_t Binop::gen() const{
        io_set_t gen_st;

        insert_name(gen_st, rhs);

        switch(op){
        case(Binop_Op::store):
                break;
        default:
                insert_name(gen_st, lhs);
                break;
        }

        return gen_st;
}

io_set_t Binop::kill() const{
        io_set_t kill_st;

        switch(op){
        case(Binop_Op::store):
                insert_name(kill_st, lhs);
                break;
        default:
                break;
        }


        return kill_st;
}

Inst_Ptr Binop::replace_vars(std::unordered_map<std::string, std::string> reg_map) const{
        return Inst_Ptr{new Binop{op,
                                sub_reg_mapping<Binop_Lhs>(reg_map, lhs),
                                sub_reg_mapping<Binop_Rhs>(reg_map, rhs)}};
}



void Binop::accept(Instruction_Visitor &v){
        v.visit(this);
}

void Binop::accept(AST_Item_Visitor &v){
        v.visit(this);
}
