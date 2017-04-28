#pragma once
#include <L2/AST/translatable.h>
#include <L2/AST/labels.h>
#include <L2/AST/marker_classes.h>
#include <vector>

namespace L2{



        class Function : public virtual AST_Item{
        public:
                Function() = default;
                Function(L2_Target_Label name, int64_t args, int64_t locals);
                Function(Function&& rhs) = default;


                std::vector<Inst_Posn> find_successors(Inst_Posn);

                liveness_sets_t make_liveness_set() const;

                void dump(std::ostream &out) const override;

                int64_t stack_args() const;
                int64_t stack_shift() const;
                void expand_stack(std::ostream&out) const;
                void shrink_stack(std::ostream&out) const;

                L2_Target_Label name;
                int64_t arguments;
                int64_t locals;
                std::vector<compiler_ptr<Instruction>> instructions;
        };
}
