#include <L2/AST/lang_constants.h>

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
