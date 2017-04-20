#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/regs.h>

namespace L2 {
        enum class Runtime_Fun{
                print,
                alloc,
                array_Error
        };

        class Runtime_Call : public Instruction{
        public:
                Runtime_Call(Runtime_Fun fun);
                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;
        private:
                Runtime_Fun fun;
        };

        class Call : public Instruction{
        public:
                Call(std::unique_ptr<Callable> fun, int64_t numargs);

                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;
        private:
                std::unique_ptr<Callable> fun;
                int64_t numargs;
        };



}  // L2
