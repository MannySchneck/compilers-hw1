#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/labels.h>

namespace L2 {

        class Goto : public Instruction{
        public:
                Goto(std::string);
                Goto(std::unique_ptr<L2_Label> target);

                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;
        private:
                std::unique_ptr<L2_Label> target;
        };


}  // L2
