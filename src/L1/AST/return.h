#pragma once

#include <L1/AST/marker_classes.h>
#include <L1/AST/labels.h>

namespace L1 {
        class Return : public Instruction{
        public:
                Return(int64_t stack_shift);
                void translate(std::ostream&) const override;
                int64_t stack_shift;
        };

}  // L1
