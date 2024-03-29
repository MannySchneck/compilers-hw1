#pragma once
#include <L1/AST/translatable.h>
#include <L1/AST/labels.h>
#include <vector>

namespace L1{

        class Function : public virtual Translatable{
        public:
                Function() = default;
                Function(L1_Target_Label name, int64_t args, int64_t locals);
                Function(Function&& rhs) = default;

                void translate(std::ostream& outfile) const override;
                int64_t stack_args() const;
                int64_t stack_shift() const;
                void expand_stack(std::ostream&out) const;
                void shrink_stack(std::ostream&out) const;

                L1_Target_Label name;
                int64_t arguments;
                int64_t locals;
                std::vector<std::unique_ptr<Instruction>> instructions;
        };
}
