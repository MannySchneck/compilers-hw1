#include <L2/AST/cmp_ops.h>

void L2::output_comparison(Translatable* lhs,
                       Translatable* rhs,
                       std::ostream& out){
        out << "cmpq ";
        rhs->translate(out);
        out << ", ";
        lhs->translate(out);

}
