#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/labels.h>

namespace L2 {
        class Return : public Instruction{
        public:
                Return(int64_t stack_shift);
                void dump(std::ostream &out) const override;

                io_set_t gen(int num_args) const override;
                io_set_t kill() const override;


                int64_t stack_shift;
        };

}  // L2
