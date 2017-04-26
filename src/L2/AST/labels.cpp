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


io_set_t L2_Label::gen(int num_args) const{
        io_set_t gen_st;

        return gen_st;
}

io_set_t L2_Label::kill() const{
        io_set_t kill_st;

        return kill_st;
}
