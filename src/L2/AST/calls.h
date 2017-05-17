#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/instruction.h>
#include <L2/AST/regs.h>

namespace L2 {
        enum class Runtime_Fun{
                print,
                alloc,
                array_Error
        };

        class Runtime_Call : public Instruction{
        public:
                Runtime_Call(Runtime_Fun fun, int64_t numargs);
                void dump(std::ostream &out) const override;

                io_set_t gen() const override;
                io_set_t kill() const override;


                Inst_Ptr replace_vars(std::unordered_map<std::string, std::string> reg_map) const override;

                void accept(Instruction_Visitor &v) override;
                void accept(AST_Item_Visitor &v) override;
        private:
                Runtime_Fun fun;
                int64_t numargs;
        };

        class Call : public Instruction{
        public:
                Call(compiler_ptr<Callable> fun, int64_t numargs);

                void dump(std::ostream &out) const override;

                io_set_t gen() const override;
                io_set_t kill() const override;


                Inst_Ptr replace_vars(std::unordered_map<std::string, std::string> reg_map) const override;

                void accept(Instruction_Visitor &v) override;
                void accept(AST_Item_Visitor &v) override;
                compiler_ptr<Callable> fun;
        private:
                int64_t numargs;
        };

}  // L2
