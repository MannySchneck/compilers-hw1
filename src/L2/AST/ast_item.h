#pragma once

#include <ostream>
#include <memory>

namespace L2{

        class AST_Item{
        public:
                virtual void dump(std::ostream &out) const = 0;
                virtual ~AST_Item() {};
        };

        template <typename T>
        using compiler_ptr = std::unique_ptr<T>;

        using ASTPtr = std::unique_ptr<AST_Item>;

} // L2
