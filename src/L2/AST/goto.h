#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/labels.h>
#include <L2/AST/jump.h>


namespace L2 {

        class Goto : public Instruction,
                     public Jump{
        public:
                Goto(std::string);
                Goto(compiler_ptr<L2_Label> target);

                void dump(std::ostream &out) const override;

                io_set_t gen() const override;
                io_set_t kill() const override;

                std::vector<Inst_Posn> find_successors(Inst_Posn pos,
                                                        std::vector<Inst_Ptr>& instrs)  const override;

                L2_Label get_target() const;

        private:
                compiler_ptr<L2_Label> target;
        };


}  // L2
