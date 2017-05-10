#include <L2/AST/labels.h>
#include <vector>
#include <L2/AST/goto.h>

using namespace L2;

Goto::Goto(std::string lab) :
        target(new L2_Label{lab}){};

Goto::Goto(compiler_ptr<L2_Label> lab) :
        target(std::move(lab)){}

void Goto::dump(std::ostream &out) const{
        out << "(";
        out << "goto ";
        target->dump(out);
        out << ")";
}

L2_Label Goto::get_target() const{
        return *target;
}

io_set_t Goto::gen() const{
        io_set_t gen_st;

        return gen_st;
}

io_set_t Goto::kill() const{
        io_set_t kill_st;

        return kill_st;
}

std::vector<Inst_Posn> Goto::find_successors(Inst_Posn pos,
                                             std::vector<Inst_Ptr> & instrs) {
        return{find_target(instrs.begin(), instrs.end(), *target)};
}

// no computed goto

Inst_Ptr Goto::replace_vars(std::unordered_map<std::string, std::string> reg_map) const{
        throw std::logic_error("nope, didn't implement Goto");
        return Inst_Ptr{};
}
