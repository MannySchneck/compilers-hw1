#include <L2/AST/cond_jump.h>
#include <L2/AST/cmp_ops.h>
#include <L2/AST/int_literal.h>
#include <L2/AST/regs.h>

using namespace L2;

Cond_Jump::Cond_Jump(Cmp_Op cmp,
                     compiler_ptr<Value_Source> cmp_lhs,
                     compiler_ptr<Value_Source> cmp_rhs,
                     L2_Label true_target,
                     L2_Label false_target) :
        cmp(cmp),
        cmp_lhs(std::move(cmp_lhs)),
        cmp_rhs(std::move(cmp_rhs)),
        true_target(true_target),
        false_target(false_target){}







void Cond_Jump::dump(std::ostream &out) const{
        out << "(";
        out << "cjump ";
        cmp_lhs->dump(out); out << " ";

        switch(cmp){
        case(Cmp_Op::less):
                out << "<";
                break;
        case(Cmp_Op::less_Equal):
                out << "<=";
                break;
        case(Cmp_Op::equal):
                out << "=";
                break;
        }

        out << " "; cmp_rhs->dump(out);
        out << " "; true_target.dump(out);
        out << " "; false_target.dump(out);
        out << ")";
}
