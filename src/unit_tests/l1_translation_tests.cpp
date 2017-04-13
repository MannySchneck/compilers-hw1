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
                Reg reg("rdi");
                std::unique_ptr<Binop_Rhs> rdi(new Memory_Ref(reg, 0));
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
