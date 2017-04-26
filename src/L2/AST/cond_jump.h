#pragma once
#include <L2/AST/marker_classes.h>

#include <string>
#include <ostream>
#include <L2/AST/labels.h>
#include <L2/AST/cmp_ops.h>

namespace L2{
        class Cond_Jump : public Instruction{
        public:
                Cond_Jump(Cmp_Op cmp,
                          compiler_ptr<Value_Source> cmp_lhs,
                          compiler_ptr<Value_Source> cmp_rhs,
                          L2_Label true_target,
                          L2_Label false_target);

                void dump(std::ostream &out) const override;


                void output_const_cjump(int result,
                                        const L2_Label& t_target,
                                        const L2_Label& f_target,
                                        std::ostream& out) const;

                void gen_cjump_text(Translatable* lhs,
                                    Translatable* rhs,
                                    L2_Label t_t,
                                    L2_Label f_t,
                                    std::string jmp,
                                    std::ostream& out) const;

                io_set_t gen(int num_args) const override;
                io_set_t kill() const override;

        private:
                Cmp_Op cmp;
                compiler_ptr<Value_Source> cmp_lhs;
                compiler_ptr<Value_Source> cmp_rhs;
                L2_Label true_target;
                L2_Label false_target;
        };

}
