#pragma once

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

        protected:
                Instruction() = default;

                template<typename T>
                compiler_ptr<T> sub_reg_mapping(std::unordered_map<std::string,
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
        compiler_ptr<T> Instruction::sub_reg_mapping(std::unordered_map<std::string,
                                                     std::string> reg_map,
                                                     compiler_ptr<T> id_maybe){

                // Fuck
                Memory_Ref* mem_ptr;
                if((mem_ptr = dynamic_cast<Memory_Ref*>(id_maybe.get()))){
                        return compiler_ptr<T>{new Memory_Ref{
                                        compiler_ptr<Writable_Reg>{new Reg{reg_map.
                                                at(mem_ptr->get_base()->get_name())}},
                                                mem_ptr->get_offset()}};
                }

                Var* var_ptr;
                if((var_ptr = dynamic_cast<Var*>(id_maybe.get()))){
                        return compiler_ptr<T>{new Writable_Reg{reg_map.
                                                at(var_ptr->get_name())}}; // Writable_Reg == GPRs
                }

                return id_maybe;
        }

}
