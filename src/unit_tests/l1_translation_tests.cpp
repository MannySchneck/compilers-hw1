#include "../L1.h"
#include "../catch.hpp"
#include <sstream>
#include <iostream>

using namespace L1;


TEST_CASE("Label translation"){
        std::stringstream ss;

        L1::L1_Label lab(":go");

        lab.translate(ss);

        REQUIRE(ss.str() == "_go");
}

TEST_CASE("memory Ref"){
        std::stringstream ss;

        Memory_Ref mem({"rax"}, 5);

        mem.translate(ss);

        REQUIRE(ss.str() == "5(%rax)");
}

TEST_CASE("Store instruction"){
        std::stringstream ss;

        SECTION("Register to register"){
                std::unique_ptr<Writable> rdi(new Writable_Reg("rdi"));
                std::unique_ptr<Binop_Rhs> r12(new Writable_Reg("r12"));

                Binop b(Binop_Op::store, std::move(rdi), std::move(r12));

                b.translate(ss);

                REQUIRE(ss.str() == "movq %r12, %rdi");
        }

        SECTION("Immediate to Reg"){
                std::unique_ptr<Binop_Lhs> rdi(new Writable_Reg("rdi"));
                std::unique_ptr<Binop_Rhs> i(new Integer_Literal("22"));

                Binop b(Binop_Op::store, std::move(rdi), std::move(i));

                b.translate(ss);

                REQUIRE(ss.str() == "movq $22, %rdi");
        }

        SECTION("Memory to Reg"){
                Reg base("rdi");
                std::unique_ptr<Binop_Lhs> mem(new Memory_Ref(base, 0));
                std::unique_ptr<Binop_Rhs> reg(new Writable_Reg("rax"));

                Binop b(Binop_Op::store, std::move(mem), std::move(reg));

                b.translate(ss);

                REQUIRE(ss.str() == "movq %rax, 0(%rdi)");
        }

        SECTION("Reg to Memory Add"){
                Reg base("rdi");
                std::unique_ptr<Binop_Lhs> mem(new Memory_Ref(base, 0));
                std::unique_ptr<Binop_Rhs> reg(new Writable_Reg("rax"));

                Binop b(Binop_Op::add_Assign, std::move(mem), std::move(reg));

                b.translate(ss);

                REQUIRE(ss.str() == "addq %rax, 0(%rdi)");
        }

}

TEST_CASE("Shift ops"){
        std::stringstream ss;
        SECTION("register"){
                std::unique_ptr<Value_Source> rcx( new Reg("rcx"));
                std::unique_ptr<Writable_Reg> rax( new Writable_Reg("rax"));

                Shop(Shop_Op::left_Shift, std::move(rax), std::move(rcx)).translate(ss);

                REQUIRE(ss.str() == "salq %cl, %rax");
        }

        SECTION("Immediate int"){
                std::unique_ptr<Value_Source> i( new Integer_Literal("3"));
                std::unique_ptr<Writable_Reg> rax( new Writable_Reg("rax"));

                Shop(Shop_Op::right_Shift, std::move(rax), std::move(i)).translate(ss);

                REQUIRE(ss.str() == "sarq $3, %rax");
        }
}

TEST_CASE("store comparison"){
        std::stringstream ss;

        SECTION("reg reg"){
                std::unique_ptr<Value_Source> lhs(new Writable_Reg("r13"));
                std::unique_ptr<Value_Source> rhs(new Reg("rsi"));
                Writable_Reg target("r15");

                Comparison_Store{Cmp_Op::equal,
                                std::move(lhs),
                                std::move(rhs),
                                target}.translate(ss);

                REQUIRE(ss.str() ==
                        "cmpq %rsi, %r13\n\t"
                        "sete %r15b\n\t"
                        "movzbq %r15b, %r15");
        }

        SECTION("reg immediate"){
                std::unique_ptr<Value_Source> lhs(new Writable_Reg("r13"));
                std::unique_ptr<Value_Source> rhs(new Integer_Literal("5"));
                Writable_Reg target("rdx");

                Comparison_Store{Cmp_Op::less,
                                std::move(lhs),
                                std::move(rhs),
                                target}.translate(ss);

                REQUIRE(ss.str() ==
                        "cmpq $5, %r13\n\t"
                        "setl %dl\n\t"
                        "movzbq %dl, %rdx");

        }

        SECTION("immediate immediate"){
                std::unique_ptr<Value_Source> lhs(new Integer_Literal("6"));
                std::unique_ptr<Value_Source> rhs(new Integer_Literal("5"));
                Writable_Reg target("rdx");

                Comparison_Store{Cmp_Op::less,
                                std::move(lhs),
                                std::move(rhs),
                                target}.translate(ss);

                REQUIRE(ss.str() == "movq $0, %rdx");
        }
        SECTION("immediate reg"){
                std::unique_ptr<Value_Source> lhs(new Integer_Literal("5"));
                std::unique_ptr<Value_Source> rhs(new Writable_Reg("r13"));
                Writable_Reg target("rdx");

                Comparison_Store{Cmp_Op::less_Equal,
                                std::move(lhs),
                                std::move(rhs),
                                target}.translate(ss);

                REQUIRE(ss.str() ==
                        "cmpq $5, %r13\n\t"
                        "setge %dl\n\t"
                        "movzbq %dl, %rdx");
                }
}

TEST_CASE("register translation"){
        std::stringstream ss;
        Reg rax("rax");

        rax.translate(ss);

        REQUIRE(ss.str() == "%rax");
}

TEST_CASE("Function Translation"){
        std::stringstream ss;

        SECTION("no instructions"){
                L1_Label flab(":funfunfun");
                L1::Function f{flab, 0, 1};

                std::unique_ptr<Return> ret{};

                f.translate(ss);

                REQUIRE(ss.str() ==
                        "_funfunfun"
                        "\n\taddq $8, %rsp"
                        "\n\tretq");
        }

        SECTION("Spill args"){
                L1_Label flab(":funfunfun");
                L1::Function f{flab, 7, 1};

                std::unique_ptr<Return> ret{};

                f.translate(ss);

                REQUIRE(ss.str() ==
                        "_funfunfun"
                        "\n\taddq $16, %rsp"
                        "\n\tretq");

        }
}

TEST_CASE("Empty program gives the expected prelude"){
        std::stringstream ss;
        L1::Program p(":go");

        p.translate(ss);

        // std::cout << "here's what it does:" << std::endl;
}
