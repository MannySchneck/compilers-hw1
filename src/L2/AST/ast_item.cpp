#include <L2/AST/ast_item.h>
#include <L2/AST/L2.h>

using namespace L2;

///////////////////////////////////////////////////////////////////////////////
//                                Instructions                               //
///////////////////////////////////////////////////////////////////////////////
void Translate_Visitor::visit(Binop* item){
        out << "(";
        item->lhs->accept(*this);
        out << " ";
        out << item->stringify_binop_op(item->op);
        out << " ";
        item->rhs->accept(*this);
        out << ")";
}

void Translate_Visitor::visit(Call* item){
        item->dump(out);
}

void Translate_Visitor::visit(Runtime_Call* item){
        item->dump(out);
}

void Translate_Visitor::visit(Comparison_Store* item){
        item->dump(out);
}

void Translate_Visitor::visit(Cond_Jump* item){
        item->dump(out);
}

void Translate_Visitor::visit(Goto* item){
        item->dump(out);
}

void Translate_Visitor::visit(L2_Label* item){
        item->dump(out);
}

void Translate_Visitor::visit(LEA* item){
        item->dump(out);
}

void Translate_Visitor::visit(Monop* item){
        item->dump(out);
}

void Translate_Visitor::visit(Shop* item){
        item->dump(out);
}

void Translate_Visitor::visit(Return* item){
        item->dump(out);
}

///////////////////////////////////////////////////////////////////////////////
//                                 structures                                //
///////////////////////////////////////////////////////////////////////////////
void Translate_Visitor::visit(Function* item){
        this->locals = item->locals;
        out << "\t(";
        item->name.dump(out);
        out << "\n";
        out << "\t\t" << item->arguments << " " << item->locals << "\n";
        for(auto i : item->instructions){
                out << "\t\t";
                i->accept(*this);
                out << "\n";
        }

        out << "\t)";
}

void Translate_Visitor::visit(Program* item){
        out << "(";
        item->entryPointLabel.dump(out);
        out << "\n\n";

        for(auto f : item->functions){
                f->accept(*this);
                out << "\n\n";
        }
        out << ")";
}

///////////////////////////////////////////////////////////////////////////////
//                                   atoms                                   //
///////////////////////////////////////////////////////////////////////////////
void Translate_Visitor::visit(L2_ID* item){
        item->dump(out);
}

void Translate_Visitor::visit(Integer_Literal* item){
        item->dump(out);
}

void Translate_Visitor::visit(Memory_Ref* item){
        item->dump(out);
}

void Translate_Visitor::visit(Stack_Arg* item){
        // This is lazy and bad
        out << "(";
        out << "mem";
        out << " ";
        out << "rsp";
        out << " ";
        out << locals * 8 + item->offset;
        out << ")";
}
