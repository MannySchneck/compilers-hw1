#include <L2/AST/labels.h>
#include <string>
#include <ostream>

using namespace L2;

L2_Label::L2_Label() {};

L2::L2_Label::L2_Label(std::string label) :
        labelName(label){};



L2_Target_Label::L2_Target_Label() = default;
L2_Target_Label::L2_Target_Label(std::string name) :
        L2_Label(name) {};


void L2_Label::dump(std::ostream &out) const{
        out << labelName;
}


void L2_Target_Label::dump(std::ostream &out) const{
        L2_Label::dump(out);
}


io_set_t L2_Label::gen() const{
        io_set_t gen_st;

        return gen_st;
}

io_set_t L2_Label::kill() const{
        io_set_t kill_st;

        return kill_st;
}

Inst_Ptr L2_Target_Label::replace_vars(std::unordered_map<std::string, std::string> reg_map) const{
        return Inst_Ptr{new L2_Target_Label{labelName}};
}


Inst_Ptr L2_Label::replace_vars(std::unordered_map<std::string, std::string> reg_map) const{
        return Inst_Ptr{new L2_Label{labelName}};
}

void L2_Label::accept(Instruction_Visitor &v){
        v.visit(this);
}

void L2_Label::accept(AST_Item_Visitor &v){
        v.visit(this);
}
