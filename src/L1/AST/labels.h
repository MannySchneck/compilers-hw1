#pragma once
#include <L1/AST/marker_classes.h>

#include <string>
#include <ostream>

namespace L1 {

        class L1_Label :
                public Source,
                public Callable,
                public Instruction
        {
        public:
                L1_Label();
                L1_Label(std::string label);

                void translate(std::ostream&) const override;
                std::string labelName;
        };

        class L1_Target_Label :
                public L1_Label
        {
        public:
                L1_Target_Label();
                L1_Target_Label(std::string label);

                void translate(std::ostream&) const override;
        };

}  // L1
