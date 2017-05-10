#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/instruction.h>
#include <L2/AST/regs.h>

namespace L2 {

        class LEA : public Instruction{
        public:
                LEA(compiler_ptr<Writable> target,
                    compiler_ptr<Writable> base,
                    compiler_ptr<Writable> offset,
                    int64_t mult);

                void dump(std::ostream &out) const override;

                io_set_t gen() const override;
                io_set_t kill() const override;


                Inst_Ptr replace_vars(std::unordered_map<std::string, std::string> reg_map) const override;
        private:
                compiler_ptr<Writable> target;
                compiler_ptr<Writable> base;

                compiler_ptr<Writable> offset;
                int64_t mult; // 0 | 2 | 4 | 8
        };


}  // L2
