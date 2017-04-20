#pragma once

#include <L2/AST/ast_item.h>

#include <ostream>
#include <memory>


namespace L2{

        class Translatable : public AST_Item{
        public:
                virtual void translate(std::ostream&) const= 0;
                virtual ~Translatable() {};
        };

        using TrPtr = std::unique_ptr<Translatable>;
}
