#pragma once

#include <vector>
#include <memory>
#include <boost/optional.hpp>


namespace L2 {
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


        class L2_ID;
        class instruction;
        class AST_Item;

        class Instruction_Visitor{
        public:
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
        };

        class Get_Ids_Visitor :
                public Instruction_Visitor{
        public:
                virtual void visit(Binop* instr) override;
                virtual void visit(Call* instr) override;
                virtual void visit(Runtime_Call* instr) override;
                virtual void visit(Comparison_Store* instr) override;
                virtual void visit(Cond_Jump* instr) override;
                virtual void visit(Goto* instr) override;
                virtual void visit(L2_Label* instr) override;
                virtual void visit(LEA* instr) override;
                virtual void visit(Monop* instr) override;
                virtual void visit(Shop* instr) override;
                virtual void visit(Return* instr) override;


                std::vector<std::string> result;

        private:
                void extract_id(std::shared_ptr<AST_Item> id_maybe);
                void maybe_push_id(std::shared_ptr<AST_Item> id_maybe);

        };
}
