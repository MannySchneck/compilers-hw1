#pragma once
#include <ostream>
#include <memory>
#include <fstream>
//#include <prettyprint.hpp>

namespace L2{

        class AST_Item_Visitor;

        class AST_Item{
        public:
                virtual void dump(std::ostream &out) const = 0;
                virtual void accept(AST_Item_Visitor &v) = 0;
                virtual ~AST_Item() {};
        };

        template <typename T>
        using compiler_ptr = std::shared_ptr<T>;

        using ASTPtr = compiler_ptr<AST_Item>;

        class Binop;
        class Call;
        class Runtime_Call;
        class Comparison_Store;
        class Cond_Jump;
        class Goto;
        class L2_Label;
        class LEA;
        class Monop;
        class Shop;
        class Return;


        class Integer_Literal;
        class L2_ID;
        class Stack_Arg;
        class Memory_Ref;

        class instruction;
        class Program;
        class Function;


        class AST_Item_Visitor{
        public:

                // Instructions
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
                virtual void visit(Return* instr) = 0;

                // structures
                virtual void visit(Function* instr) = 0;
                virtual void visit(Program* instr) = 0;

                // atoms
                virtual void visit(L2_ID* instr) = 0;
                virtual void visit(Integer_Literal* instr) = 0;
                virtual void visit(Memory_Ref* instr) = 0;
                virtual void visit(Stack_Arg* instr) = 0;
        };

        class Translate_Visitor :
                public AST_Item_Visitor{
        public:
                explicit Translate_Visitor(std::ostream &out) :
                        out(out),
                        locals(-1){}

                void visit(Binop* instr);
                void visit(Call* instr);
                void visit(Runtime_Call* instr);
                void visit(Comparison_Store* instr);
                void visit(Cond_Jump* instr);
                void visit(Goto* instr);
                void visit(L2_Label* instr);
                void visit(LEA* instr);
                void visit(Monop* instr);
                void visit(Shop* instr);
                void visit(Return* instr);

                // structures
                void visit(Function* instr);
                void visit(Program* instr);

                // atoms
                void visit(L2_ID* instr);
                void visit(Integer_Literal* instr);
                void visit(Memory_Ref* instr);
                void visit(Stack_Arg* instr);

        private:
                std::ostream& out;
                int64_t locals;
        };
} // L2
