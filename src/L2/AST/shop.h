#pragma once
#include <L2/AST/marker_classes.h>
#include <L2/AST/regs.h>

#include <string>
#include <ostream>

namespace L2{
        enum class Shop_Op{
                // Shift ops
                right_Shift,
                left_Shift
        };

        class Shop : public Instruction
        {
        public:
                Shop(Shop_Op,
                     compiler_ptr<Writable>,
                     compiler_ptr<Value_Source>);

                void dump(std::ostream &out) const override;

                io_set_t gen() const override;
                io_set_t kill() const override;

        private:
                Shop_Op op;
                compiler_ptr<Writable> lhs;
                compiler_ptr<Value_Source> rhs;
        };
}
