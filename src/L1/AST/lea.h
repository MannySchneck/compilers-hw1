#pragma once

#include <L1/AST/marker_classes.h>
#include <L1/AST/regs.h>

namespace L1 {

        class LEA : public Instruction{
        public:
                LEA(Writable_Reg target, Writable_Reg base, Writable_Reg offset, int64_t mult);
                void translate(std::ostream&) const override;
        private:
                Writable_Reg target;
                Writable_Reg base;

                Writable_Reg offset;
                int64_t mult; // 0 | 2 | 4 | 8
        };


}  // L1
