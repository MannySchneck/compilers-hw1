#pragma once

#include <ostream>
#include <memory>

namespace L2{

        class AST_Item{
        public:
                virtual void dump(std::ostream &out) const = 0;
                virtual ~AST_Item() {};
        };

        using ASTPtr = std::unique_ptr<AST_Item>;

} // L2
