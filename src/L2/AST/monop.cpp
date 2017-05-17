#include <L2/AST/monop.h>
#include <iostream>

using namespace L2;

Monop::Monop(Monop_Op op, compiler_ptr<Writable> target) :
        op(op),
        target(std::move(target)){};


void Monop::dump(std::ostream &out) const{
        out << "(";
        target->dump(out);
        switch(op){
        case(Monop_Op::inc):
                out << "++";
                break;
        case(Monop_Op::dec):
                out << "--";
                break;
        }
        out << ")";
}


io_set_t Monop::gen() const{
        io_set_t gen_st;

        insert_name(gen_st, target);

        return gen_st;
}

io_set_t Monop::kill() const{
        io_set_t kill_st;

        return kill_st;
}

Inst_Ptr Monop::replace_vars(std::unordered_map<std::string, std::string> reg_map) const{
        return Inst_Ptr{new Monop{op,
                                sub_reg_mapping<Writable>(reg_map, target)}};

        return Inst_Ptr{};
}

void Monop::accept(Instruction_Visitor &v){
        v.visit(this);
}

void Monop::accept(AST_Item_Visitor &v){
        v.visit(this);
}
