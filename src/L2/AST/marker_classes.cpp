#include <L2/AST/marker_classes.h>
#include <string>
#include <L2/AST/memory_ref.h>

using namespace L2;

L2_ID::L2_ID(std::string name) :
        name(name){}


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

std::vector<Inst_Posn>
Instruction::find_successors(Inst_Posn pos,
                             std::vector<Inst_Ptr> &instrs) const{
        return std::vector<Inst_Posn>{std::next(pos)};
}

std::ostream& L2::operator<<(std::ostream& out, Instruction &inst){
        inst.dump(out);
        return out;
}
