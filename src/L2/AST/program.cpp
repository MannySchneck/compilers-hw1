#include <L2/AST/program.h>
#include <iostream>

using namespace L2;

const char*  Program:: prog_prefix = ".text\n.globl go\n\n";

const std::array<char*, 6> Program::callee_saves = {"rbx",
                                                        "rbp",
                                                        "r12",
                                                        "r13",
                                                        "r14",
                                                        "r15"};


Program::Program(std::string label) :
        entryPointLabel(label){}


void Program::allocate_registers(){
        std::vector<compiler_ptr<Function>> new_fancy_funs;

        for(auto f : functions){
                new_fancy_funs.push_back(f->allocate_registers());
        }

        functions = new_fancy_funs;
}

void Program::dump(std::ostream &out) const{
        out << "prog!";
}

void Program::accept(AST_Item_Visitor &v){
        v.visit(this);
}
