#include <L2/AST/program.h>

const char*  L2::Program:: prog_prefix = ".text\n.globl go\n\n";

const std::array<char*, 6> L2::Program::callee_saves = {"rbx",
                                                        "rbp",
                                                        "r12",
                                                        "r13",
                                                        "r14",
                                                        "r15"};


L2::Program::Program(std::string label) :
        entryPointLabel(label){}

void L2::Program::dump(std::ostream &out) const{
        out << "prog!";
}
