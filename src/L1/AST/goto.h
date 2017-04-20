#pragma once

#include <L1/AST/marker_classes.h>
#include <L1/AST/labels.h>

namespace L1 {

        class Goto : public Instruction{
        public:
                Goto(std::string);
                Goto(std::unique_ptr<L1_Label> target);

                void translate(std::ostream&) const override;
        private:
                std::unique_ptr<L1_Label> target;
        };


}  // L1
