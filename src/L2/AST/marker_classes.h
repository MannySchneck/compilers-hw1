#pragma once

#include <L2/AST/translatable.h>
#include <set>
#include <string>
#include <vector>
#include <ostream>


namespace L2{

        class L2_ID;


        class Binop_Rhs : public virtual AST_Item{
        protected:
                Binop_Rhs() = default;
        };

        class Binop_Lhs : public virtual AST_Item{
        protected:
                Binop_Lhs() = default;
        };

        // s
        ////////////////////////////////////////////////////////////////////////////////
        class Source :
                public virtual Binop_Rhs,
                public virtual AST_Item{
        };

        ////////////////////////////////////////////////////////////////////////////////
        // u
        class Callable : public virtual AST_Item{
        protected:
                Callable() {};
        };

        //x
        ////////////////////////////////////////////////////////////////////////////////
        class X:
                public virtual AST_Item{};

        // t
        ////////////////////////////////////////////////////////////////////////////////
        class Value_Source : public virtual Source{
        protected:
                Value_Source() {};
        };


        //w
        ////////////////////////////////////////////////////////////////////////////////
        class Writable :
                public virtual AST_Item,
                public Callable,
                public Binop_Lhs,
                public virtual Value_Source,
                public virtual Binop_Rhs
        {
        protected:
                Writable(){};
        };


        ////////////////////////////////////////////////////////////////////////////////
        class L2_ID : public virtual AST_Item {
        public:
                explicit L2_ID(std::string name);
                std::string get_name();
                std::string name;
        };
}
