#include <L1/AST/program.h>
#include <L1/AST/L1_consts.h>

const char*  L1::Program:: prog_prefix = ".text\n.globl go\n\n";

const std::array<char*, 6> L1::Program::callee_saves = {"rbx",
                                                        "rbp",
                                                        "r12",
                                                        "r13",
                                                        "r14",
                                                        "r15"};


L1::Program::Program(std::string label) :
        entryPointLabel(label){}

void L1::Program::translate(std::ostream &out) const {
        out << prog_prefix;

        out << "go:" << std::endl;

        for(auto it = callee_saves.cbegin(); it != callee_saves.cend(); ++it){
                out << "\t" << "pushq" << "\t" << "%" << *it << "\n";
        }

        out << "\t\n";
        out << "call ";
        entryPointLabel.translate(out);
        out << "\t\n\t\n";

        for(auto it = callee_saves.crbegin(); it != callee_saves.crend(); ++it){
                out << "\t" << "popq" << "\t" << "%" << *it << "\n";
        }

        out << "\tretq";

        for(auto& fun : functions){
                out << "\n";
                fun->translate(out);
        }
        out << "\n";
}
