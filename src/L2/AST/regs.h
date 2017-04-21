#pragma once
#include <L2/AST/marker_classes.h>

#include <string>
#include <ostream>
#include <unordered_map>

namespace L2{
        class Reg :
                public Value_Source
        {
        public:
                Reg(std::string name);

                Reg get_eight_bit() const;

                void dump(std::ostream &out) const override;

        private:
                std::string name;
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

