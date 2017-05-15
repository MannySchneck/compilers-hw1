#pragma once

#include <L2/AST/instruction_visitor.h>
#include <L2/AST/marker_classes.h>
#include <unordered_map>


namespace L2{
        using io_set_t = std::set<std::string>;

        using liveness_sets_t =  std::pair<std::vector<L2::io_set_t>,
                                           std::vector<L2::io_set_t>>;



        class Instruction : public virtual AST_Item{
        public:
                io_set_t in;
                io_set_t out;

                virtual io_set_t gen() const = 0;
                virtual io_set_t kill() const = 0;


                using Inst_Ptr = compiler_ptr<Instruction>;
                using Inst_Posn = std::vector<Inst_Ptr>::iterator;

                bool one_iteration_calc_io_sets(Inst_Posn,
                                                std::vector<Inst_Ptr> & instrs);

                virtual std::vector<Inst_Posn>
                find_successors(Inst_Posn pos,
                                std::vector<Inst_Ptr> & instr);

                void insert_name(io_set_t &io_set, const compiler_ptr<AST_Item> &id) const;

                virtual Inst_Ptr replace_vars(std::unordered_map<std::string, std::string> reg_map) const = 0;

///////////////////////////////////////////////////////////////////////////////
//                                 Do a Visitor...                           //
///////////////////////////////////////////////////////////////////////////////
                virtual void accept(Instruction_Visitor &v) = 0;

        protected:
                Instruction() = default;

                template<typename T>
                static compiler_ptr<T> sub_reg_mapping(std::unordered_map<std::string,
                                                std::string> reg_map,
                                                compiler_ptr<T> id_maybe);
        };

        using Inst_Ptr = Instruction::Inst_Ptr;

        using Inst_Posn = Instruction::Inst_Posn;


        std::ostream& operator<<(std::ostream& out, Instruction &inst);

        class Var;

}


#include <L2/AST/var.h>
#include <L2/AST/regs.h>
#include <L2/AST/memory_ref.h>


namespace L2{

        template<typename T>
        compiler_ptr<T> sub_var(std::unordered_map<std::string, std::string> reg_map,
                                std::string name){
                        return compiler_ptr<T>{
                                new Writable_Reg{(reg_map.at(name))}}; // Writable_Reg == GPRs
        }

        template<typename T>
        compiler_ptr<T> Instruction::sub_reg_mapping(std::unordered_map<std::string,
                                                     std::string> reg_map,
                                                     compiler_ptr<T> id_maybe){

                // Badness here. Break var case into fun. Use on mem. (or, fix the design...)
                Memory_Ref* mem_ptr;
                if((mem_ptr = dynamic_cast<Memory_Ref*>(id_maybe.get()))){
                        return compiler_ptr<T>{new Memory_Ref{
                                        sub_var<Reg>(reg_map,
                                                     mem_ptr->get_base()->get_name()),
                                                mem_ptr->get_offset()}};
                }

                Var* var_ptr;
                if((var_ptr = dynamic_cast<Var*>(id_maybe.get()))){
                        return sub_var<T>(reg_map, var_ptr->get_name());
                }

                return id_maybe;
        }

}
