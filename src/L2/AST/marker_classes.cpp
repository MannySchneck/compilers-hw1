#include <L2/AST/marker_classes.h>
#include <string>

using namespace L2;

L2_ID::L2_ID(std::string name) :
        name(name){}


std::string L2_ID::get_name(){
        return name;
}

void L2_ID::accept(AST_Item_Visitor &v){
        v.visit(this);
}
