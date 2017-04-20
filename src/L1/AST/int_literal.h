#pragma once

#include <L1/AST/translatable.h>
#include <L1/AST/marker_classes.h>

namespace L1{
        class Integer_Literal :
                public Value_Source
        {
        public:
                Integer_Literal(std::string);
                void translate(std::ostream&) const override;
                int64_t value;
        };
}
