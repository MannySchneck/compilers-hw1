#pragma once

#include <L1/AST/translatable.h>
#include <L1/AST/marker_classes.h>
#include <L1/AST/regs.h>

namespace L1{
        class Memory_Ref :
                public Binop_Rhs,
                public Binop_Lhs,
                public virtual Translatable {
public:
        Memory_Ref(Reg base, int64_t offset);
        void translate(std::ostream&) const override;
private:
        Reg base;
        int64_t offset;
};

}
