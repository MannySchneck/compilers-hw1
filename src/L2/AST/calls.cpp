#include <L2/AST/calls.h>

using namespace L2;


Call::Call(compiler_ptr<Callable> fun, int64_t numargs) :
        fun(std::move(fun)),
        numargs(numargs){}

Runtime_Call::Runtime_Call(Runtime_Fun fun) :
        fun(fun){}




void Call::dump(std::ostream &out) const{
        out << "(call ";
        fun->dump(out);
        out << " " << numargs;
        out << ")";
}

void Runtime_Call::dump(std::ostream &out) const{
        out << "(call ";
        switch(fun){
        case(Runtime_Fun::print):
                "print 1";
                break;
        case(Runtime_Fun::alloc):
                "alocate 2";
                break;
        case(Runtime_Fun::array_Error):
                "array-error 2";
                break;
        }


        out << " ";
        out << ")";

}


io_set_t Call::gen() const{
        io_set_t gen_st;

        return gen_st;
}

io_set_t Call::kill() const{
        io_set_t kill_st;

        return kill_st;
}

io_set_t Runtime_Call::gen() const{
        io_set_t gen_st;

        return gen_st;
}

io_set_t Runtime_Call::kill() const{
        io_set_t kill_st;

        return kill_st;
}
