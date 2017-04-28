#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/instruction.h>

namespace L2{
        class Var : public Writable,
                    public L2_ID{
        public:
                Var(std::string name);

                void dump(std::ostream &out) const override;
        private:
        };
}
