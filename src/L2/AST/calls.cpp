#include <L2/AST/calls.h>

using namespace L2;


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
                out << "allocate";
                break;
        case (Runtime_Fun::array_Error):
                out << "array_error";
                break;

        }
}


void Call::dump(std::ostream &out) const{
        out << "ACK!";
}

void Runtime_Call::dump(std::ostream &out) const{
        out << "ACK!";
}


