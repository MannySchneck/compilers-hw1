#pragma once

#include <L2/AST/translatable.h>
#include <L2/AST/marker_classes.h>
#include <L2/AST/regs.h>

namespace L2{
        class Memory_Ref :
                public Binop_Rhs,
                public Binop_Lhs {
        public:
                Memory_Ref(compiler_ptr<X> base, int64_t offset);
                void dump(std::ostream &out) const override;
        private:
                compiler_ptr<X> base;
                int64_t offset;
        };

}
