#pragma once

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <stdint.h>

namespace L1{

        enum class Binop_Op{
                store,
                // arithmetic ops
                add_Assign,
                sub_Assign,
                mult_Assign,
                bit_And_Assign,
                // Shift ops
                right_Shift,
                left_Shift
        // comparators
        };

        enum class Monop_Op{
                inc,
                dec
        };

        enum class Runtime_Fun{
                print,
                alloc,
                array_Error
        };

        class Translatable{
        public:
                virtual void translate() = 0;
                virtual ~Translatable();
        };

        class Binop_Rhs{
        protected:
                Binop_Rhs();
        };


        // s
        class Source :
                public Binop_Rhs,
                public Translatable{
        };

        class Callable{
        protected:
                Callable() {};
        };

        // t
        class Value_Source : public Source{
        public:
                void translate() override;
        };

        class Comparison : public Value_Source{
        public:
                void translate() override;
        private:
                Value_Source lhs;
                Value_Source rhs;
        };

        // N
        class Integer_Literal :
                public Value_Source
        {
                int64_t value;
        };

        class Writable
        {
        protected:
                Writable(){};
        };

        // x
        class Reg :
                public Value_Source
        {
        public:
                Reg(std::string name);
        private:
                std::string name;
        };

        // w
        class Writable_Reg :
                public Reg,
                Writable,
                Callable
        {};

        // a
        class Argument_Reg : Reg
        {};

        // label
        class L1_Label :
                public Source,
                Callable
        {
        public:
                void translate() override;
        private:
                std::string labelName;
        };

        class Memory_Ref :
                public Binop_Rhs,
                public Translatable
        {
        public:
                void translate() override;
        private:
                Reg base;
                int64_t offset;
        };

        // instructions
        class Instruction : public Translatable{
        public:
                void translate() override;
        };

        class Binop : public Instruction
        {
        private:
                Writable lhs;
                Binop_Op op;
                Binop_Rhs rhs;
        };

        class Monop : public Instruction{
        public:
                void translate() override;
        private:
                Monop_Op op;
                Writable_Reg target;
        };

        // jumps
        class L1_Goto : public Instruction{
        public:
                void translate() override;
        private:
                L1_Label target;
        };

        class Cond_Jump : public Instruction{
        public:
                void translate() override;
        private:
                Value_Source lhs;
                Value_Source rhs;
                L1_Label true_target;
                L1_Label false_target;
        };

        class Return : public Instruction{
        public:
                void translate() override;
        };

        // calls
        class Runtime_Call : public Instruction{
        public:
                void translate() override;
        private:
                Runtime_Fun fun;
        };

        class Call : public Instruction{
        public:
                void translate() override;
        private:
                Callable fun;
                int64_t numargs;
        };

        // LEA (poor lonely child)
        class Load_Effective_Addr : public Instruction{
        public:
                void translate() override;
        private:
                Writable_Reg target;
                Writable_Reg base;
                Writable_Reg Offset;
                int64_t mult; // 0 | 2 | 4 | 8
        };

        // function
        class Function : public Translatable{
        public:
                Function(Function&& rhs) = default;

                void translate() override;

                std::string name;
                int64_t arguments;
                int64_t locals;
        };

        // entry point (program)
        class Program : public Translatable{
        public:
                Program () = default;
                Program(Program&& rhs) = default;

                void translate() override;

                std::string entryPointLabel;
                std::vector<std::unique_ptr<L1::Function>> functions;
        };


        Program L1_parse_file (Program &p,char *fileName);

} // L1
