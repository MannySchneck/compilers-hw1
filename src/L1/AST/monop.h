#pragma once

#include <L1/AST/translatable.h>
#include <L1/AST/marker_classes.h>
#include <L1/AST/regs.h>

namespace L1{
        enum class Monop_Op{
                inc,
                dec
        };

        class Monop : public Instruction{
        public:
                Monop(Monop_Op op, std::unique_ptr<Writable_Reg> target);
                void translate(std::ostream&) const override;
        private:
                Monop_Op  op;
                std::unique_ptr<Writable_Reg> target;
        };

}
