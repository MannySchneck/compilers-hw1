#pragma once
#include <L2/AST/marker_classes.h>

#include <string>
#include <ostream>
#include <unordered_map>

namespace L2{
        class Reg :
                public virtual Value_Source,
                public L2_ID
        {
        public:
                Reg(std::string name);

                Reg get_eight_bit() const;

                void dump(std::ostream &out) const override;

                virtual ~Reg();

        private:
                static const std::unordered_map<std::string, std::string> eight_bit_name;

        };

// w
        class Writable_Reg :
                public Reg,
                public Writable
        {
        public:
                Writable_Reg(std::string);
                void dump(std::ostream &out) const override;
        };



// a
        class Argument_Reg : public Reg
        {};



}

