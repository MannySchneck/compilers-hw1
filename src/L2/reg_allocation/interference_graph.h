#pragma once

#include <iterator>

// Takes liveness sets and generates an interference graph. 

enum class GPR_Color{
        rdi,
        rsi,
        rdx,
        rcx,
        r8,
        r9,
        rax,
        r10,
        r11,
        r12,
        r13,
        r14,
        r15,
        rbp,
        rbx,
        end,
        begin = rdi,
};

class Interference_Graph{
public:
private:
};

class IG_Node{

};
