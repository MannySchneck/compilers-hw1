#pragma once

#include <L2/AST/ast_item.h>
#include <L2/AST/marker_classes.h>

namespace L2{
        class Var : public Writable{
        public:
                Var(std::string name);

                void dump(std::ostream &out) const override;
        private:
                std::string name;
        };
}
