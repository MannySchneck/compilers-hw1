#pragma once

#include <L2/AST/translatable.h>
#include <L2/AST/marker_classes.h>
#include <L2/AST/regs.h>

namespace L2{
        class Memory_Ref :
                public Binop_Rhs,
                public Binop_Lhs,
                public virtual Translatable {
        public:
                Memory_Ref(Reg base, int64_t offset);
                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;
        private:
                Reg base;
                int64_t offset;
        };

}
