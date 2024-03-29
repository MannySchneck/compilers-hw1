#pragma once

#include <L2/AST/translatable.h>
#include <L2/AST/marker_classes.h>
#include <L2/AST/instruction.h>
#include <L2/AST/regs.h>

namespace L2{
        enum class Monop_Op{
                inc,
                dec
        };

        class Monop : public Instruction{
        public:
                Monop(Monop_Op op, compiler_ptr<Writable> target);
                void dump(std::ostream &out) const override;

                io_set_t gen() const override;
                io_set_t kill() const override;

        private:
                Monop_Op  op;
                compiler_ptr<Writable> target;
        };

}
