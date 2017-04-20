#include <L1/AST/labels.h>
#include <string>
#include <ostream>

using namespace L1;

L1_Label::L1_Label() {};

L1::L1_Label::L1_Label(std::string label) :
        labelName(label){};

void L1::L1_Label::translate(std::ostream &out) const {
        out << "_" << labelName.substr(1, labelName.size());
}

L1_Target_Label::L1_Target_Label() = default;
L1_Target_Label::L1_Target_Label(std::string name) :
        L1_Label(name) {};
void L1_Target_Label::translate(std::ostream &out) const {
        L1_Label::translate(out);
        out << ":";
}
