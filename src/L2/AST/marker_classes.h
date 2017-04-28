#pragma once

#include <L2/AST/translatable.h>
#include <unordered_set>
#include <string>
#include <vector>
#include <ostream>


namespace L2{

        class L2_ID;

        using io_set_t = std::unordered_set<std::string>;

        using liveness_sets_t =  std::pair<std::vector<L2::io_set_t>,
                                           std::vector<L2::io_set_t>>;



        ////////////////////////////////////////////////////////////////////////////////
        class Instruction : public virtual AST_Item{
        public:
                io_set_t in;
                io_set_t out;

                virtual io_set_t gen() const = 0;
                virtual io_set_t kill() const = 0;


                using Inst_Ptr = compiler_ptr<Instruction>;
                using Inst_Posn = std::vector<Inst_Ptr>::const_iterator;

                virtual std::vector<Inst_Posn>
                find_successors(Inst_Posn pos,
                                std::vector<Inst_Ptr> & instrs) const;

                void insert_name(io_set_t &io_set, const compiler_ptr<AST_Item> &id) const;
        protected:
                Instruction() = default;
        };

        using Inst_Ptr = Instruction::Inst_Ptr;

        using Inst_Posn = Instruction::Inst_Posn;


        std::ostream& operator<<(std::ostream& out, Instruction &inst);
        //std::ostream& operator<<(std::ostream& out, Instruction &inst);

        ////////////////////////////////////////////////////////////////////////////////
        class Binop_Rhs : public virtual AST_Item{
        protected:
                Binop_Rhs() = default;
        };

        class Binop_Lhs : public virtual AST_Item{
        protected:
                Binop_Lhs() = default;
        };

        // s
        ////////////////////////////////////////////////////////////////////////////////
        class Source :
                public virtual Binop_Rhs,
                public virtual AST_Item{
        };

        ////////////////////////////////////////////////////////////////////////////////
        // u
        class Callable : public virtual AST_Item{
        protected:
                Callable() {};
        };

        //x
        ////////////////////////////////////////////////////////////////////////////////
        class X:
                public virtual AST_Item{};

        // t
        ////////////////////////////////////////////////////////////////////////////////
        class Value_Source : public virtual Source{
        protected:
                Value_Source() {};
        };


        //w
        ////////////////////////////////////////////////////////////////////////////////
        class Writable :
                public virtual AST_Item,
                public Callable,
                public Binop_Lhs,
                public virtual Value_Source,
                public virtual Binop_Rhs
        {
        protected:
                Writable(){};
        };


        ////////////////////////////////////////////////////////////////////////////////
        class L2_ID : public virtual AST_Item {
        public:
                L2_ID(std::string name);
                std::string get_name();
                std::string name;
        };
}
