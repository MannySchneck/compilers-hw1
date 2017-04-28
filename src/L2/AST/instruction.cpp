#include <L2/AST/instruction.h>
#include <L2/AST/memory_ref.h>
#include <algorithm>
#include <iostream>

using namespace L2;

void Instruction::insert_name(io_set_t &io_set, const compiler_ptr<AST_Item> &id) const{
        std::string name;
        if(auto p = dynamic_cast<L2_ID*>(id.get())){
                name = p->name;
        } else if(auto p = dynamic_cast<Memory_Ref*>(id.get())){
                name = p->get_base()->name;
        }

        if(name != "rsp" && name != ""){
                io_set.insert(name);
        }

}

//XXX This is horrifically slow.
//XXX This is a pile of shit.
bool Instruction::one_iteration_calc_io_sets(Inst_Posn pos,
                                             std::vector<Inst_Ptr> &insts){
        bool changed{false};

        io_set_t new_out;
        for(auto s : find_successors(pos, insts)){
                new_out.insert((*s)->in.begin(), (*s)->in.end());
        }

        if(new_out != out){
                out = (new_out);
                changed = true;
        }

        io_set_t kill_st = kill();

        io_set_t new_in{gen()};

        io_set_t out_sub_kill{out};

        for(auto elt : kill_st){
                out_sub_kill.erase(elt);
        }

        new_in.insert(out_sub_kill.begin(), out_sub_kill.end());



        if(new_in != in){
                in = (new_in);
                changed = true;
        }

        return changed;
}

std::vector<Inst_Posn>
Instruction::find_successors(Inst_Posn pos,
                             std::vector<Inst_Ptr> &insts) {

        auto successor = std::next(pos);
        if(successor == insts.end()) return {};

        return {std::next(pos)};
}



std::ostream& L2::operator<<(std::ostream& out, Instruction &inst){
        inst.dump(out);
        return out;
}
