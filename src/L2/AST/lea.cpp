#include <L2/AST/lea.h>
#include <set>
#include <string>

using namespace L2;

LEA::LEA(compiler_ptr<Writable> target,
         compiler_ptr<Writable> base,
         compiler_ptr<Writable> offset,
         int64_t mult) :
        target(target),
        base(base),
        offset(offset),
        mult(mult)
{
        if((mult % 2) || mult > 8){
                throw std::invalid_argument("mult must be even, and 8 or less");
        }
}

void LEA::dump(std::ostream &out) const{
        out << "(";
        target->dump(out);
        out << " ";
        out << "@";
        out << " ";
        base->dump(out);
        out << " ";
        offset->dump(out);
        out << " " << mult;
        out << ")";

}

io_set_t LEA::gen() const{
        io_set_t gen_st;

        insert_name(gen_st, base);
        insert_name(gen_st, offset);

        return gen_st;
}

io_set_t LEA::kill() const{
        io_set_t kill_st;

        insert_name(kill_st, target);

        return kill_st;
}

Inst_Ptr LEA::replace_vars(std::unordered_map<std::string, std::string> reg_map) const{
        throw std::logic_error("nope, didn't implement LEA");
        return Inst_Ptr{};
}
