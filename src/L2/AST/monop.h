#pragma once

#include <L2/AST/translatable.h>
#include <L2/AST/marker_classes.h>
#include <L2/AST/regs.h>

namespace L2{
        enum class Monop_Op{
                inc,
                dec
        };

        class Monop : public Instruction{
        public:
                Monop(Monop_Op op, std::unique_ptr<Writable_Reg> target);
                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;
        private:
                Monop_Op  op;
                std::unique_ptr<Writable_Reg> target;
        };

}
