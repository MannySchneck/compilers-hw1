#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/regs.h>

namespace L2 {

        class LEA : public Instruction{
        public:
                LEA(Writable_Reg target,
                    Writable_Reg base,
                    Writable_Reg offset,
                    int64_t mult);

                void dump(std::ostream &out) const override;

                io_set_t gen(int num_args) const override;
                io_set_t kill() const override;
        private:
                Writable_Reg target;
                Writable_Reg base;

                Writable_Reg offset;
                int64_t mult; // 0 | 2 | 4 | 8
        };


}  // L2
