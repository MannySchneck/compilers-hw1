#include <array>
const char* prog_prefix = ".text\n.globl go\n";

const std::array<char*, 6>& callee_saves(){
        static const std::array<char*, 6> callee_saves_table = {"rbx",
                                                                "rbp",
                                                                "r12",
                                                                "r13",
                                                                "r14",
                                                                "r15"};
        return callee_saves_table;
}
