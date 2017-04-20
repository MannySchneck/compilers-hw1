#include <L2/AST/labels.h>
#include <string>
#include <ostream>

using namespace L2;

L2_Label::L2_Label() {};

L2::L2_Label::L2_Label(std::string label) :
        labelName(label){};

void L2::L2_Label::translate(std::ostream &out) const {
        out << "_" << labelName.substr(1, labelName.size());
}

L2_Target_Label::L2_Target_Label() = default;
L2_Target_Label::L2_Target_Label(std::string name) :
        L2_Label(name) {};
void L2_Target_Label::translate(std::ostream &out) const {
        L2_Label::translate(out);
        out << ":";
}

void L2_Target_Label::dump(std::ostream &out) const{
        out << "ACK!";
}

void L2_Label::dump(std::ostream &out) const{
        out << "ACK!";
}

