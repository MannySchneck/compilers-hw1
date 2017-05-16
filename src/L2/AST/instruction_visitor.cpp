#include <L2/AST/instruction_visitor.h>
#include <L2/AST/L2.h>

void Get_Ids_Visitor::maybe_push_id(std::shared_ptr<AST_Item> id_maybe){

        L2_ID* id_ptr = nullptr;
        if((id_ptr = dynamic_cast<L2_ID*>(id_maybe.get()))){
                result.push_back(id_ptr->get_name());
        }
}

void Get_Ids_Visitor::extract_id(std::shared_ptr<AST_Item> id_maybe){
        Memory_Ref* mem_ptr = nullptr;
        if(( mem_ptr = dynamic_cast<Memory_Ref*>(id_maybe.get()))){
                auto base = mem_ptr->get_base();
                maybe_push_id(base);
        } else{
                maybe_push_id(id_maybe);
        }

}

void Get_Ids_Visitor::visit(Binop* instr){
        extract_id(instr->lhs);
        extract_id(instr->rhs);
}

void Get_Ids_Visitor::visit(Call* instr){
        extract_id(instr->fun);
}

void Get_Ids_Visitor::visit(Runtime_Call* instr){
        return;
}

void Get_Ids_Visitor::visit(Comparison_Store* instr){
        extract_id(instr->lhs);
        extract_id(instr->rhs);
        extract_id(instr->target);
}

void Get_Ids_Visitor::visit(Cond_Jump* instr){
        extract_id(instr->cmp_lhs);
        extract_id(instr->cmp_rhs);
}

void Get_Ids_Visitor::visit(Goto* instr){
        return;
}

void Get_Ids_Visitor::visit(L2_Label* instr){
        return;
}

void Get_Ids_Visitor::visit(LEA* instr){
        extract_id(instr->target);
        extract_id(instr->base);
        extract_id(instr->offset);
}

void Get_Ids_Visitor::visit(Monop* instr){
        extract_id(instr->target);
}

void Get_Ids_Visitor::visit(Shop* instr){
        extract_id(instr->lhs);
        extract_id(instr->rhs);
}

void Get_Ids_Visitor::visit(Return* instr){
        return;
}

