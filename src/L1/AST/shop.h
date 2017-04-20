#pragma once
#include <L1/AST/marker_classes.h>
#include <L1/AST/regs.h>

#include <string>
#include <ostream>

namespace L1{
        enum class Shop_Op{
                // Shift ops
                right_Shift,
                        left_Shift
                        };

        class Shop : public Instruction
        {
        public:
                Shop(Shop_Op, std::unique_ptr<Writable_Reg>, std::unique_ptr<Value_Source>);
                void translate(std::ostream&) const override;
        private:
                Shop_Op op;
                std::unique_ptr<Writable_Reg> lhs;
                std::unique_ptr<Value_Source> rhs;
        };
}
