#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <type_traits>
#include <exception>
#include <unordered_map>

class NotImplementedException : public std::logic_error
{
public:
        NotImplementedException() :
                std::logic_error("Function not implemented. Moron.")
                {};
};

namespace L1{


        enum class Binop_Op{
                store,
                // arithmetic ops
                        add_Assign,
                        sub_Assign,
                        mult_Assign,
                        bit_And_Assign,
                        };

        enum class Shop_Op{
                // Shift ops
                right_Shift,
                        left_Shift
                        };

        enum class Cmp_Op{
                equal,
                        less,
                        less_Equal,
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
                virtual void translate(std::ostream&) const= 0;
                virtual ~Translatable() {};
        };

        using TrPtr = std::unique_ptr<Translatable>;

        class Binop_Rhs : public virtual Translatable{
        protected:
                Binop_Rhs() = default;
        };

        class Binop_Lhs : public virtual Translatable{
        protected:
                Binop_Lhs() = default;
        };


        // s
        class Source :
                public Binop_Rhs,
                public virtual Translatable{
        };

        class Callable : public virtual Translatable{
        protected:
                Callable() {};
        };

        class Writable :
                public virtual Translatable,
                public Binop_Lhs
        {
        protected:
                Writable(){};
        };

        // t
        class Value_Source : public Source{
        protected:
                Value_Source() {};
        };

///////////////////////////////////////////////////////////////////////////////
//                               Concrete types                              //
///////////////////////////////////////////////////////////////////////////////

// N
        class Integer_Literal :
                public Value_Source
        {
        public:
                Integer_Literal(std::string);
                void translate(std::ostream&) const override;
                int64_t value;
        };

// x
        class Reg :
                public Value_Source
        {
        public:
                Reg(std::string name);

                Reg get_eight_bit() const;

                void translate(std::ostream&) const override;
        private:
                std::string name;
                static const std::unordered_map<std::string, std::string> eight_bit_name;

        };

// w
        class Writable_Reg :
                public Reg,
                public Callable,
                public Writable
        {
        public:
                Writable_Reg(std::string);
                void translate(std::ostream&) const override;
        };

        class Comparison_Store : public virtual Translatable
        {
        public:
                Comparison_Store(Cmp_Op op,
                                 std::unique_ptr<Value_Source> lhs,
                                 std::unique_ptr<Value_Source> rhs,
                                 Writable_Reg target);

                void translate(std::ostream&) const override;
        private:
                Cmp_Op op;
                Writable_Reg target;
                std::unique_ptr<Value_Source> lhs;
                std::unique_ptr<Value_Source> rhs;
        };


// a
        class Argument_Reg : public Reg
        {};


// instructions
        class Instruction : public virtual Translatable{
        public:
                //void translate(std::ostream&) const override;
        };

// label
        class L1_Label :
                public Source,
                public Callable,
                public Instruction
        {
        public:
                L1_Label();
                L1_Label(std::string label);

                void translate(std::ostream&) const override;
                std::string labelName;
        };


        class Shop : public Instruction
        {
        public:
                Shop(Shop_Op, std::unique_ptr<Writable_Reg>, std::unique_ptr<Value_Source>);
                void translate(std::ostream&) const override;
        private:
                Shop_Op op;
                std::unique_ptr<Writable_Reg> lhs;
                std::unique_ptr<Value_Source> rhs;
        };

        class Binop : public Instruction
        {
        public:
                Binop(Binop_Op op, std::unique_ptr<Binop_Lhs> lhs, std::unique_ptr<Binop_Rhs> rhs);
                void translate(std::ostream&) const override;

        private:
                std::unique_ptr<Binop_Lhs> lhs;
                Binop_Op op;
                std::unique_ptr<Binop_Rhs> rhs;
        };

        class Memory_Ref :
                public Binop_Rhs,
                public Binop_Lhs,
                public virtual Translatable {
        public:
                Memory_Ref(Reg base, int64_t offset);
                void translate(std::ostream&) const override;
        private:
                Reg base;
                int64_t offset;
        };

        class Monop : public Instruction{
        public:
                Monop(Monop_Op op, std::unique_ptr<Writable_Reg> target);
                void translate(std::ostream&) const override;
        private:
                Monop_Op  op;
                std::unique_ptr<Writable_Reg> target;
        };

// jumps
        class Goto : public Instruction{
        public:
                Goto(std::string);

                void translate(std::ostream&) const override;
        private:
                L1_Label target;
        };

        class Cond_Jump : public Instruction{
        public:
                Cond_Jump(Cmp_Op cmp,
                          std::unique_ptr<Value_Source> Cmp_Lhs,
                          std::unique_ptr<Value_Source> Cmp_Rhs,
                          L1_Label true_target,
                          L1_Label false_target);

                void translate(std::ostream&) const override;
        private:
                Cmp_Op cmp;
                std::unique_ptr<Value_Source> Cmp_Lhs;
                std::unique_ptr<Value_Source> Cmp_Rhs;
                L1_Label true_target;
                L1_Label false_target;
        };

        class Return : public Instruction{
        public:
                void translate(std::ostream&) const override;
        };

// calls
        class Runtime_Call : public Instruction{
        public:
                Runtime_Call(Runtime_Fun fun);
                void translate(std::ostream&) const override;
        private:
                Runtime_Fun fun;
        };

        class Call : public Instruction{
        public:
                Call(std::unique_ptr<Callable> fun, int64_t numargs);

                void translate(std::ostream&) const override;
        private:
                std::unique_ptr<Callable> fun;
                int64_t numargs;
        };


// LEA (poor lonely child)
        class LEA : public Instruction{
        public:
                LEA(Writable_Reg target, Writable_Reg base, Writable_Reg offset, int64_t mult);
                void translate(std::ostream&) const override;
        private:
                Writable_Reg target;
                Writable_Reg base;

                Writable_Reg offset;
                int64_t mult; // 0 | 2 | 4 | 8
        };

// function
        class Function : public virtual Translatable{
        public:
                Function() = default;
                Function(L1_Label name, int64_t args, int64_t locals);
                Function(Function&& rhs) = default;

                void translate(std::ostream& outfile) const override;
                int64_t stack_args() const;

                L1_Label name;
                int64_t arguments;
                int64_t locals;
                std::vector<std::unique_ptr<Instruction>> instructions;
        };

// entry point (program)
        class Program : public virtual Translatable{
        public:

                Program() = default;
                Program(Program&& rhs) = default;

                Program(std::string label);

                ~Program() override = default;

                void translate(std::ostream&) const override;

                L1_Label entryPointLabel;
                std::vector<std::unique_ptr<L1::Function>> functions;

        private:
        };


        Program L1_parse_file (std::string);
} // L1
