#pragma once

#include <L2/AST/marker_classes.h>
namespace L2{
#include <L2/AST/var.h>
        class Var : public Writable,
                    public L2_ID{
        public:
                explicit Var(std::string name);

                void dump(std::ostream &out) const override;
        private:
        };
}


