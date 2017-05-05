#include <L2/AST/lang_constants.h>
#include <vector>

const std::vector<std::string> Lang_Constants::arg_regs = {
        "rdi",
        "rsi",
        "rdx",
        "rcx",
        "r8",
        "r9"
};

const std::vector<std::string> Lang_Constants::callee_saves = {
        "r12",
        "r13",
        "r14",
        "r15",
        "rbp",
        "rbx"
};

const std::vector<std::string> Lang_Constants::caller_saves = {
        "r8",
        "r9",
        "r10",
        "r11",
        "rax",
        "rdi",
        "rsi",
        "rdx",
        "rcx"
};


// Ask Andreas, how do I initialize this and have it be const?
// need to wrap it in a class
std::vector<std::string> Lang_Constants::regs_vector;

std::unordered_set<std::string> Lang_Constants::regs_set;

Lang_Constants::_init Lang_Constants::_initializer;

