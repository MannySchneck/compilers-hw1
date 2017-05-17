#include <L2/AST/ast_item.h>
#include <L2/AST/L2.h>

using namespace L2;

///////////////////////////////////////////////////////////////////////////////
//                                Instructions                               //
///////////////////////////////////////////////////////////////////////////////
void Translate_Visitor::visit(Binop* item){
        out << "didn't implement Binop... SAD!\n";
}

void Translate_Visitor::visit(Call* item){
        out << "didn't implement Call... SAD!\n";
}

void Translate_Visitor::visit(Runtime_Call* item){
        out << "didn't implement Runtime_Call... SAD!\n";
}

void Translate_Visitor::visit(Comparison_Store* item){
        out << "didn't implement Comparison_Store... SAD!\n";
}

void Translate_Visitor::visit(Cond_Jump* item){
        out << "didn't implement Cond_Jump... SAD!\n";
}

void Translate_Visitor::visit(Goto* item){
        out << "didn't implement Goto... SAD!\n";
}

void Translate_Visitor::visit(L2_Label* item){
        out << "didn't implement L2_Label... SAD!\n";
}

void Translate_Visitor::visit(LEA* item){
        out << "didn't implement LEA... SAD!\n";
}

void Translate_Visitor::visit(Monop* item){
        out << "didn't implement Monop... SAD!\n";
}

void Translate_Visitor::visit(Shop* item){
        out << "didn't implement Shop... SAD!\n";
}

void Translate_Visitor::visit(Return* item){
        out << "didn't implement Return... SAD!\n";
}

///////////////////////////////////////////////////////////////////////////////
//                                 structures                                //
///////////////////////////////////////////////////////////////////////////////
void Translate_Visitor::visit(Function* item){
        out << "didn't implement Function... SAD!\n";
}

void Translate_Visitor::visit(Program* item){
        out << "didn't implement Program... SAD!\n";
}

///////////////////////////////////////////////////////////////////////////////
//                                   atoms                                   //
///////////////////////////////////////////////////////////////////////////////
void Translate_Visitor::visit(L2_ID* item){
        out << "didn't implement L2_ID... SAD!\n";
}

void Translate_Visitor::visit(Integer_Literal* item){
        out << "didn't implement Integer_Literal... SAD!\n";
}

void Translate_Visitor::visit(Memory_Ref* item){
        out << "didn't implement Memory_Ref... SAD!\n";
}

void Translate_Visitor::visit(Stack_Arg* item){
        out << "didn't implement Stack_Arg... SAD!\n";
}
