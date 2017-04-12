#include "L1.h"
#include "L1_consts.h"
#include "catch.hpp"





void L1::Program::translate(std::ostream &out) const {
        for(auto it = callee_saves().cbegin(); it != callee_saves().cend(); ++it){
                out << "\t" << "pushq" << "\t" << "%" << *it << "\n";
        }
}

void L1::Function::translate(std::ostream &out) const{
        out << "ack!";
};
