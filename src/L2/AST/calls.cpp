#include <L2/AST/calls.h>
#include <L2/AST/labels.h>
#include <L2/AST/lang_constants.h>

using namespace L2;


Call::Call(compiler_ptr<Callable> fun, int64_t numargs) :
        fun(std::move(fun)),
        numargs(numargs){}

Runtime_Call::Runtime_Call(Runtime_Fun fun, int64_t numargs) :
        fun(fun),
        numargs(numargs){
}




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
                out << "print 1";
                break;
        case(Runtime_Fun::alloc):
                out << "alocate 2";
                break;
        case(Runtime_Fun::array_Error):
                out << "array-error 2";
                break;
        }


        out << " ";
        out << ")";

}


io_set_t Call::gen() const{
        io_set_t gen_st;

        if(dynamic_cast<L2_Label*>(fun.get())){
                ;
        }
        else if(auto p = dynamic_cast<L2_ID*>(fun.get())){
                gen_st.insert(p->name);
        }

        for(int i = 0; i < numargs; i++){
                gen_st.insert(Lang_Constants::arg_regs[i]);
        }

        return gen_st;
}

io_set_t Call::kill() const{
        io_set_t kill_st;

        for(auto reg : Lang_Constants::caller_saves){
                kill_st.insert(reg);
        }

        return kill_st;
}

io_set_t Runtime_Call::gen() const{
        io_set_t gen_st;

        for(int i = 0; i < numargs; i++){
                 gen_st.insert(Lang_Constants::arg_regs[i]);
        }

        return gen_st;
}

io_set_t Runtime_Call::kill() const{
        io_set_t kill_st;

        for(auto reg : Lang_Constants::caller_saves){
                kill_st.insert(reg);
        }

        return kill_st;
}
