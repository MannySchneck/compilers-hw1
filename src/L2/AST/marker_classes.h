#pragma once

#include <L2/AST/translatable.h>


namespace L2{

        class Instruction : public virtual AST_Item{
        protected:
                Instruction() = default;
        };


        class Binop_Rhs : public virtual AST_Item{
        protected:
                Binop_Rhs() = default;
        };

        class Binop_Lhs : public virtual AST_Item{
        protected:
                Binop_Lhs() = default;
        };


          // s
        class Source :
                public virtual Binop_Rhs,
                public virtual AST_Item{
        };

        // u
        class Callable : public virtual AST_Item{
        protected:
                Callable() {};
        };

        //x
        class X:
                public virtual AST_Item{};

        //w
        class Writable :
                public virtual AST_Item,
                public Callable,
                public Binop_Lhs,
                public virtual Binop_Rhs,
                public virtual X
        {
        protected:
                Writable(){};
        };


        // t
        class Value_Source : public virtual Source,
                             public virtual X{
        protected:
                Value_Source() {};
        };
}
