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

io_set_t Binop::gen(int num_args) const{
        io_set_t gen_st;

        if(auto p = dynamic_cast<L2_ID*>(rhs.get())){
                gen_st.insert(p->name);
        }

        if(auto p = dynamic_cast<L2_ID*>(lhs.get())){
                switch(op){
                case(Binop_Op::store):
                        break;
                default:
                        gen_st.insert(p->name);
                        break;
                }
        }
        else {
                throw std::logic_error("WAT. lhs is not an lvalue");
        }

        return gen_st;
}

io_set_t Binop::kill() const{
        io_set_t kill_st;

        if(auto p = dynamic_cast<L2_ID*>(lhs.get())){
                switch(op){
                case(Binop_Op::store):
                        kill_st.insert(p->name);
                        break;
                default:
                        break;
                }
        }


        return kill_st;
}
