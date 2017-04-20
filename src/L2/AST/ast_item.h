#pragma once

#include <ostream>

namespace L2{

        class AST_Item{
        public:
                virtual void dump(std::ostream &out) const = 0;
                virtual ~AST_Item() {};
        };

} // L2
