#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/labels.h>

namespace L2 {

        class Goto : public Instruction{
        public:
                Goto(std::string);
                Goto(compiler_ptr<L2_Label> target);

                void dump(std::ostream &out) const override;
        private:
                compiler_ptr<L2_Label> target;
        };


}  // L2
