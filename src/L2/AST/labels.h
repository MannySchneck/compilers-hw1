#pragma once
#include <L2/AST/marker_classes.h>

#include <string>
#include <ostream>

namespace L2 {

        class L2_Label :
                public Source,
                public Callable,
                public Instruction
        {
        public:
                L2_Label();
                L2_Label(std::string label);

                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;
                std::string labelName;
        };

        class L2_Target_Label :
                public L2_Label
        {
        public:
                L2_Target_Label();
                L2_Target_Label(std::string label);

                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;
        };

}  // L2
