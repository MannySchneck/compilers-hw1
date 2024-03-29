#pragma once

#include <L2/AST/ast_item.h>

#include <ostream>
#include <memory>


namespace L2{

        class Translatable {
        public:
                virtual void translate(std::ostream&) const= 0;
                virtual ~Translatable() {};
        };

        using TrPtr = compiler_ptr<Translatable>;
}
