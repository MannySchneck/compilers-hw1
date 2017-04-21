#pragma once
#include <L2/AST/marker_classes.h>
#include <memory>

namespace L2 {

        enum class Binop_Op{
                store,
                // arithmetic ops
                add_Assign,
                sub_Assign,
                mult_Assign,
                bit_And_Assign,
                };

        class Binop : public Instruction
        {
        public:
                Binop(Binop_Op op, std::unique_ptr<Binop_Lhs> lhs, std::unique_ptr<Binop_Rhs> rhs);

                void dump(std::ostream &out) const override;

        private:
                std::unique_ptr<Binop_Lhs> lhs;
                Binop_Op op;
                std::unique_ptr<Binop_Rhs> rhs;
        };


}  // L2
