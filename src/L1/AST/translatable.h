#pragma once

#include <ostream>
#include <memory>


namespace L1{

        class Translatable{
        public:
                virtual void translate(std::ostream&) const= 0;
                virtual ~Translatable() {};
        };

        using TrPtr = std::unique_ptr<Translatable>;
}
