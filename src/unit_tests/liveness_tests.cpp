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

                REQUIRE(store->gen() == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

        SECTION("binop store with immediate"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::store,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Integer_Literal("4")})};

                std::unordered_set<std::string> gen_st{};
                std::unordered_set<std::string> kill_st{"rax"};

                REQUIRE(store->gen() == gen_st);
                REQUIRE(store->kill() == kill_st);
        }


        SECTION("binop add"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::add_Assign,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

                std::unordered_set<std::string> gen_st{"rbx", "rax"};
                std::unordered_set<std::string> kill_st{};

                REQUIRE(store->gen() == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

        SECTION("binop mult with constant"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::mult_Assign,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Integer_Literal("4")})};

                std::unordered_set<std::string> gen_st{"rax"};
                std::unordered_set<std::string> kill_st{};

                REQUIRE(store->gen() == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

        SECTION("Call"){
                std::unordered_set<std::string> caller_saves;

                for(auto reg : Lang_Constants::caller_saves){
                        caller_saves.insert(reg);
                }

                compiler_ptr<Instruction> call{new Call{compiler_ptr<Callable>{new Writable_Reg("rax")}, 4}};

                std::unordered_set<std::string> gen_st{"rax", "rdi", "rsi", "rcx", "rdx"};
                std::unordered_set<std::string> kill_st = caller_saves;

                REQUIRE(call->gen() == gen_st);
                REQUIRE(call->kill() == kill_st);

        }

        SECTION("Call with label"){
                std::unordered_set<std::string> caller_saves;

                for(auto reg : Lang_Constants::caller_saves){
                        caller_saves.insert(reg);
                }

                compiler_ptr<Instruction> call{new Call{compiler_ptr<Callable>{new L2_Label("rax")}, 4}};

                std::unordered_set<std::string> gen_st{"rdi", "rsi", "rcx", "rdx"};
                std::unordered_set<std::string> kill_st = caller_saves;

                REQUIRE(call->gen() == gen_st);
                REQUIRE(call->kill() == kill_st);

        }

        SECTION("runtime call"){
                std::unordered_set<std::string> caller_saves;

                for(auto reg : Lang_Constants::caller_saves){
                        caller_saves.insert(reg);
                }

                compiler_ptr<Instruction> call{new Runtime_Call{Runtime_Fun::print, 1}};

                std::unordered_set<std::string> gen_st{"rdi"};
                std::unordered_set<std::string> kill_st = caller_saves;

                REQUIRE(call->gen() == gen_st);
                REQUIRE(call->kill() == kill_st);

        }

        SECTION("Cmp store with rsp"){
                compiler_ptr<Instruction> cmp_str{new Comparison_Store{Cmp_Op::equal,
                                        compiler_ptr<Value_Source>{new Reg{"rsp"}},
                                        compiler_ptr<Value_Source>{new Reg{"rax"}},
                                                compiler_ptr<Writable>{new Var{"blerp"}}}};

                std::unordered_set<std::string> gen_st{"rax"};
                std::unordered_set<std::string> kill_st{"blerp"};

                REQUIRE(cmp_str->gen() == gen_st);
                REQUIRE(cmp_str->kill() == kill_st);
        }

        SECTION("Cmp store with all vars"){
                compiler_ptr<Instruction> cmp_str{new Comparison_Store{Cmp_Op::equal,
                                        compiler_ptr<Value_Source>{new Var{"rspy"}},
                                        compiler_ptr<Value_Source>{new Var{"raxy"}},
                                                compiler_ptr<Writable>{new Var{"blerp"}}}};

                std::unordered_set<std::string> gen_st{"raxy", "rspy"};
                std::unordered_set<std::string> kill_st{"blerp"};

                REQUIRE(cmp_str->gen() == gen_st);
                REQUIRE(cmp_str->kill() == kill_st);
        }
        SECTION("Cmp store immediates"){
                compiler_ptr<Instruction> cmp_str{new Comparison_Store{Cmp_Op::equal,
                                        compiler_ptr<Value_Source>{new Integer_Literal{"6"}},
                                        compiler_ptr<Value_Source>{new Integer_Literal{"5"}},
                                                compiler_ptr<Writable>{new Var{"blerp"}}}};

                std::unordered_set<std::string> gen_st{};
                std::unordered_set<std::string> kill_st{"blerp"};

                REQUIRE(cmp_str->gen() == gen_st);
                REQUIRE(cmp_str->kill() == kill_st);
        }


}
