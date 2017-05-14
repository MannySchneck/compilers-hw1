#pragma once
#include <L2/AST/L2.h>

class Instruction_Vistor{
        virtual void visit(Binop* instr) = 0;
        virtual void visit(Call* instr) = 0;
        virtual void visit(Runtime_Call* instr) = 0;
        virtual void visit(Comparison_Store* instr) = 0;
        virtual void visit(Cond_Jump* instr) = 0;
        virtual void visit(Goto* instr) = 0;
        virtual void visit(L2_Label* instr) = 0;
        virtual void visit(LEA* instr) = 0;
        virtual void visit(Monop* instr) = 0;
        virtual void visit(Shop* instr) = 0;
        virtual void visit(* instr) = 0;
        virtual void visit(Return* instr) = 0;
};

