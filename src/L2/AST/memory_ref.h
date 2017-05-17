#pragma once

#include <L2/AST/translatable.h>
#include <L2/AST/marker_classes.h>
#include <L2/AST/regs.h>

namespace L2{
        class Memory_Ref :
                public Binop_Rhs,
                public Binop_Lhs {
        public:
                explicit Memory_Ref(compiler_ptr<L2_ID> base, int64_t offset);
                void dump(std::ostream &out) const override;

                compiler_ptr<L2_ID> get_base();

                int64_t get_offset(){return offset;}
                void accept(AST_Item_Visitor &v) override;

        private:
                compiler_ptr<L2_ID> base;
                int64_t offset;
        };

}
