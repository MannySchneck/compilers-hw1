#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/labels.h>

namespace L2 {
        class Return : public Instruction{
        public:
                Return(int64_t stack_shift);
                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;
                int64_t stack_shift;
        };

}  // L2
