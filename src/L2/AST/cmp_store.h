#pragma once
#include <L2/AST/cmp_ops.h>
#include <L2/AST/translatable.h>
#include <L2/AST/marker_classes.h>
#include <L2/AST/regs.h>
#include <memory>

namespace L2{

        class Comparison_Store : public Instruction
        {
        public:
                Comparison_Store(Cmp_Op op,
                                 std::unique_ptr<Value_Source> lhs,
                                 std::unique_ptr<Value_Source> rhs,
                                 Writable_Reg target);

                void dump(std::ostream &out) const override;

        private:
                void output_comparison_store(Translatable* lhs,
                                             Translatable* rhs,
                                             Writable_Reg target,
                                             std::string cmp_set,
                                             std::ostream& out) const;

                void output_mov(Writable_Reg target, int val, std::ostream& out) const;

                Cmp_Op op;
                Writable_Reg target;
                std::unique_ptr<Value_Source> lhs;
                std::unique_ptr<Value_Source> rhs;
        };

}
