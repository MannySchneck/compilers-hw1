#pragma once
#include <L1/AST/marker_classes.h>

#include <string>
#include <ostream>
#include <L1/AST/labels.h>
#include <L1/AST/cmp_ops.h>

namespace L1{
        class Cond_Jump : public Instruction{
        public:
                Cond_Jump(Cmp_Op cmp,
                          std::unique_ptr<Value_Source> Cmp_Lhs,
                          std::unique_ptr<Value_Source> Cmp_Rhs,
                          L1_Label true_target,
                          L1_Label false_target);

                void translate(std::ostream&) const override;

                void output_const_cjump(int result, const L1_Label& t_target, const L1_Label& f_target, std::ostream& out) const;

                void gen_cjump_text(Translatable* lhs,
                                    Translatable* rhs,
                                    L1_Label t_t,
                                    L1_Label f_t,
                                    std::string jmp,
                                    std::ostream& out) const;

        private:
                Cmp_Op cmp;
                std::unique_ptr<Value_Source> Cmp_Lhs;
                std::unique_ptr<Value_Source> Cmp_Rhs;
                L1_Label true_target;
                L1_Label false_target;
        };

}
