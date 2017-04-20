#pragma once

#include <L2/AST/translatable.h>
#include <L2/AST/marker_classes.h>

namespace L2{
        class Integer_Literal :
                public Value_Source
        {
        public:
                Integer_Literal(std::string);
                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;
                int64_t value;
        };
}
