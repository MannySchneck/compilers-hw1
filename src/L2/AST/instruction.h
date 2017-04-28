#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/instruction.h>

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
        protected:
                Instruction() = default;
        };

        using Inst_Ptr = Instruction::Inst_Ptr;

        using Inst_Posn = Instruction::Inst_Posn;


        std::ostream& operator<<(std::ostream& out, Instruction &inst);

}
