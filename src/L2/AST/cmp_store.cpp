#include <L2/AST/cmp_store.h>
#include <L2/AST/int_literal.h>

using namespace L2;

Comparison_Store::Comparison_Store(Cmp_Op op,
                                   compiler_ptr<Value_Source> lhs,
                                   compiler_ptr<Value_Source> rhs,
                                   Writable_Reg target) :
        op(op),
        target(target),
        lhs(std::move(lhs)),
        rhs(std::move(rhs)){}




void Comparison_Store::dump(std::ostream &out) const{
        out << "cmp stor!";
}
