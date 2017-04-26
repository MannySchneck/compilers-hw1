#pragma once
#include <L2/AST/marker_classes.h>

#include <string>
#include <ostream>

namespace L2 {

        class L2_Label :
                public virtual Source,
                public virtual Callable,
                public Instruction
        {
        public:
                L2_Label();
                L2_Label(std::string label);

                void dump(std::ostream &out) const override;
                std::string labelName;

                io_set_t gen(int num_args) const override;
                io_set_t kill() const override;


        };

        class L2_Target_Label :
                public L2_Label
        {
        public:
                L2_Target_Label();
                L2_Target_Label(std::string label);

                void dump(std::ostream &out) const override;
        };

}  // L2
