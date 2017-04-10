#pragma once

#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <stdint.h>
#include <type_traits>

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
////////////////////////////////////////////////////////////////////////////////

        class Translatable{
        public:
                virtual void translate() const = 0;
                virtual ~Translatable();
        };

        class Binop_Rhs{
        protected:
                Binop_Rhs();
        };

        class Binop_Lhs{
        protected:
                Binop_Lhs();
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
        protected:
                Value_Source() {};
        };

        template<typename lhs_t, typename rhs_t>
        class Comparison : public Translatable{
                static_assert(std::is_base_of<Value_Source, lhs_t>::value &&
                              std::is_base_of<Value_Source, rhs_t>::value,
                              "Can't do a comparison unless it's on Value_Sources");
        public:
                Comparison(lhs_t lhs, rhs_t rhs) :
                        lhs(lhs),
                        rhs(rhs){};

                void translate() const override;
        private:
                lhs_t lhs;
                rhs_t rhs;
        };

        // N
        class Integer_Literal :
                public Value_Source
        {
                void translate() const override;
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

                void translate() const override;
        private:
                std::string name;
        };

        // w
        class Writable_Reg :
                public Reg,
                Binop_Lhs,
                Writable,
                Callable
        {};

        // a
        class Argument_Reg : public Reg
        {};

        // label
        class L1_Label :
                public Source,
                Callable
        {
        public:
                void translate() const override;
        private:
                std::string labelName;
        };

        // instructions
        class Instruction : public Translatable{
        public:
                void translate() const override;
        };

        class Binop : public Instruction
        {
        private:
                Writable lhs;
                Binop_Op op;
                Binop_Rhs rhs;
        };

        class Memory_Ref :
                public Binop_Rhs,
                public Binop_Lhs,
                public Translatable
        {
        public:
                Memory_Ref(const Reg& base, int64_t offset) :
                        base(base),
                        offset(offset)
                        {};
                void translate() const override;
        private:
                Reg base;
                int64_t offset;
        };

        class Monop : public Instruction{
        public:
                void translate() const override;
        private:
                Monop_Op  op;
                Writable_Reg target;
        };

        // jumps
        class L1_Goto : public Instruction{
        public:
                void translate() const override;
        private:
                L1_Label target;
        };

        template<typename lhs_t, typename rhs_t>
        class Cond_Jump : public Instruction{
        public:
                void translate() const override;
        private:
                Comparison<lhs_t, rhs_t> cmp;
                L1_Label true_target;
                L1_Label false_target;
        };

        class Return : public Instruction{
        public:
                void translate() const override;
        };

        // calls
        class Runtime_Call : public Instruction{
        public:
                void translate() const override;
        private:
                Runtime_Fun fun;
        };

        template<typename callable_t>
        class Call : public Instruction{
                static_assert(std::is_base_of<Callable, callable_t>::value, "not a callable type dude =(");
        public:
                void translate() const override;
        private:
                callable_t fun;
                int64_t numargs;
        };

        // LEA (poor lonely child)
        class Load_Effective_Addr : public Instruction{
        public:
                void translate() const override;
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

                void translate() const override;

                std::string name;
                int64_t arguments;
                int64_t locals;
        };

        // entry point (program)
        class Program : public Translatable{
        public:
                Program () = default;
                Program(Program&& rhs) = default;

                void translate() const override;

                std::string entryPointLabel;
                std::vector<std::unique_ptr<L1::Function>> functions;

        private:
                std::fstream out_file;
        };


        Program L1_parse_file (Program &p,char *fileName);

} // L1
