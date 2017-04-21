#pragma once

#include <L2/AST/marker_classes.h>

namespace L2{
        class Stack_Arg :
                public Binop_Rhs{
        public:
                Stack_Arg(int64_t offset);

                void dump(std::ostream &out) const override;
        private:
                int64_t offset;
        };
}
