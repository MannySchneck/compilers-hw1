#pragma once

#include <L2/AST/translatable.h>


namespace L2{

        class Instruction : public virtual Translatable{
        protected:
                Instruction() = default;
        };


        class Binop_Rhs : public virtual Translatable{
        protected:
                Binop_Rhs() = default;
        };

        class Binop_Lhs : public virtual Translatable{
        protected:
                Binop_Lhs() = default;
        };


// s
        class Source :
                public Binop_Rhs,
                public virtual Translatable{
                };

        class Callable : public virtual Translatable{
        protected:
                Callable() {};
        };

        class Writable :
                public virtual Translatable,
                public Binop_Lhs
                {
                protected:
                        Writable(){};
                };

// t
        class Value_Source : public Source{
        protected:
                Value_Source() {};
        };
}


