#include <L2/AST/return.h>
#include <L2/AST/lang_constants.h>

using namespace L2;


Return::Return(int64_t stack_shift):
        stack_shift(stack_shift){}



void Return::dump(std::ostream &out) const{
        out << "(return)";
}

io_set_t Return::gen() const{
        io_set_t gen_st;

        for(auto reg : Lang_Constants::callee_saves){
                gen_st.insert(reg);
        }

        gen_st.insert("rax");
        return gen_st;
}

io_set_t Return::kill() const{
        io_set_t kill_st;

        return kill_st;
}
