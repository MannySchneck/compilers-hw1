#include <L2/AST/marker_classes.h>
#include <string>

using namespace L2;

L2_ID::L2_ID(std::string name) :
        name(name){}


void Instruction::insert_name(io_set_t &io_set, const compiler_ptr<AST_Item> &id) const{
        if(auto p = dynamic_cast<L2_ID*>(id.get())){
                if(p->name != "rsp"){
                        io_set.insert(p->name);
                }
        }

}
