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
