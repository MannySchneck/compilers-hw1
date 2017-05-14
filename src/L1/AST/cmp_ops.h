#pragma once
#include <ostream>
#include<L1/AST/translatable.h>

namespace L1{

        enum class Cmp_Op{
                equal,
                less,
                less_Equal,
                };

        enum class Cmp_Case{
                both_Int,
                flip,
                nothing
                };


        void output_comparison(Translatable* lhs,
                               Translatable* rhs,
                               std::ostream& out);
}
