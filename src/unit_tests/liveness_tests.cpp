#include <catch.hpp>
#include <L2/AST/L2.h>
#include <unordered_set>
#include <string>

using namespace L2;

TEST_CASE("Correct gen/kill "){

        SECTION("binop store"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::store,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

                std::unordered_set<std::string> gen_st{"rbx"};
                std::unordered_set<std::string> kill_st{"rax"};

                REQUIRE(store->gen(0) == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

        SECTION("binop store with immediate"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::store,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Integer_Literal("4")})};

                std::unordered_set<std::string> gen_st{};
                std::unordered_set<std::string> kill_st{"rax"};

                REQUIRE(store->gen(0) == gen_st);
                REQUIRE(store->kill() == kill_st);
        }


        SECTION("binop add"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::add_Assign,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

                std::unordered_set<std::string> gen_st{"rbx", "rax"};
                std::unordered_set<std::string> kill_st{};

                REQUIRE(store->gen(0) == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

        SECTION("binop mult with constant"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::mult_Assign,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Integer_Literal("4")})};

                std::unordered_set<std::string> gen_st{"rax"};
                std::unordered_set<std::string> kill_st{};

                REQUIRE(store->gen(0) == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

}

