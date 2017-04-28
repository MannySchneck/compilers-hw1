#include <catch.hpp>
#include <L2/AST/L2.h>
#include <L2/AST/lang_constants.h>
#include <set>
#include <string>
#include <prettyprint.hpp>
#include <L2/AST/marker_classes.h>

using namespace L2;


TEST_CASE("Correct gen/kill "){

        SECTION("binop store"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::store,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

                std::set<std::string> gen_st{"rbx"};
                std::set<std::string> kill_st{"rax"};

                REQUIRE(store->gen() == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

        SECTION("binop store with immediate"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::store,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Integer_Literal("4")})};

                std::set<std::string> gen_st{};
                std::set<std::string> kill_st{"rax"};

                REQUIRE(store->gen() == gen_st);
                REQUIRE(store->kill() == kill_st);
        }


        SECTION("binop add from memory"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::bit_And_Assign,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Memory_Ref(compiler_ptr<Reg>{new Reg{"rbx"}},
                                                                                                 8)})};

                std::set<std::string> gen_st{"rbx", "rax"};
                std::set<std::string> kill_st{};

                REQUIRE(store->gen() == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

        SECTION("binop add"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::add_Assign,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

                std::set<std::string> gen_st{"rbx", "rax"};
                std::set<std::string> kill_st{};

                REQUIRE(store->gen() == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

        SECTION("binop mult with constant"){
                compiler_ptr<Instruction> store{new Binop(Binop_Op::mult_Assign,
                                                          compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                          compiler_ptr<Binop_Rhs>{new Integer_Literal("4")})};

                std::set<std::string> gen_st{"rax"};
                std::set<std::string> kill_st{};

                REQUIRE(store->gen() == gen_st);
                REQUIRE(store->kill() == kill_st);
        }

        SECTION("Call"){
                std::set<std::string> caller_saves;

                for(auto reg : Lang_Constants::caller_saves){
                        caller_saves.insert(reg);
                }

                compiler_ptr<Instruction> call{new Call{compiler_ptr<Callable>{new Writable_Reg("rax")}, 4}};

                std::set<std::string> gen_st{"rax", "rdi", "rsi", "rcx", "rdx"};
                std::set<std::string> kill_st = caller_saves;

                REQUIRE(call->gen() == gen_st);
                REQUIRE(call->kill() == kill_st);

        }

        SECTION("Call with label"){
                std::set<std::string> caller_saves;

                for(auto reg : Lang_Constants::caller_saves){
                        caller_saves.insert(reg);
                }

                compiler_ptr<Instruction> call{new Call{compiler_ptr<Callable>{new L2_Label("rax")}, 4}};

                std::set<std::string> gen_st{"rdi", "rsi", "rcx", "rdx"};
                std::set<std::string> kill_st = caller_saves;

                REQUIRE(call->gen() == gen_st);
                REQUIRE(call->kill() == kill_st);

        }

        SECTION("runtime call"){
                std::set<std::string> caller_saves;

                for(auto reg : Lang_Constants::caller_saves){
                        caller_saves.insert(reg);
                }

                compiler_ptr<Instruction> call{new Runtime_Call{Runtime_Fun::print, 1}};

                std::set<std::string> gen_st{"rdi"};
                std::set<std::string> kill_st = caller_saves;

                REQUIRE(call->gen() == gen_st);
                REQUIRE(call->kill() == kill_st);

        }

        SECTION("Cmp store with rsp"){
                compiler_ptr<Instruction> cmp_str{new Comparison_Store{Cmp_Op::equal,
                                        compiler_ptr<Value_Source>{new Reg{"rsp"}},
                                        compiler_ptr<Value_Source>{new Reg{"rax"}},
                                                compiler_ptr<Writable>{new Var{"blerp"}}}};

                std::set<std::string> gen_st{"rax"};
                std::set<std::string> kill_st{"blerp"};

                REQUIRE(cmp_str->gen() == gen_st);
                REQUIRE(cmp_str->kill() == kill_st);
        }

        SECTION("Cmp store with all vars"){
                compiler_ptr<Instruction> cmp_str{new Comparison_Store{Cmp_Op::equal,
                                        compiler_ptr<Value_Source>{new Var{"rspy"}},
                                        compiler_ptr<Value_Source>{new Var{"raxy"}},
                                                compiler_ptr<Writable>{new Var{"blerp"}}}};

                std::set<std::string> gen_st{"raxy", "rspy"};
                std::set<std::string> kill_st{"blerp"};

                REQUIRE(cmp_str->gen() == gen_st);
                REQUIRE(cmp_str->kill() == kill_st);
        }
        SECTION("Cmp store immediates"){
                compiler_ptr<Instruction> cmp_str{new Comparison_Store{Cmp_Op::equal,
                                        compiler_ptr<Value_Source>{new Integer_Literal{"6"}},
                                        compiler_ptr<Value_Source>{new Integer_Literal{"5"}},
                                                compiler_ptr<Writable>{new Var{"blerp"}}}};

                std::set<std::string> gen_st{};
                std::set<std::string> kill_st{"blerp"};

                REQUIRE(cmp_str->gen() == gen_st);
                REQUIRE(cmp_str->kill() == kill_st);
        }

        SECTION("cjump "){
                compiler_ptr<Instruction> cjump(new Cond_Jump(Cmp_Op::equal,
                                                              compiler_ptr<Value_Source>(new Reg("rax")),
                                                              compiler_ptr<Value_Source>(new Reg("rbx")),
                                                              L2_Label(":true"),
                                                              L2_Label(":false")));

                std::set<std::string> gen_st{"rax", "rbx"};
                std::set<std::string> kill_st{};

                REQUIRE(cjump->gen() == gen_st);
                REQUIRE(cjump->kill() == kill_st);
        }

        SECTION("LEA"){
                compiler_ptr<Instruction> lea(new LEA(compiler_ptr<Writable>(new Var("blerp")),
                                                      compiler_ptr<Writable>(new Writable_Reg{"rax"}),
                                                      compiler_ptr<Writable>(new Var("schmoop")),
                                                      4));

                std::set<std::string> gen_st{"rax", "schmoop"};
                std::set<std::string> kill_st{"blerp"};

                REQUIRE(lea->gen() == gen_st);
                REQUIRE(lea->kill() == kill_st);

        }

        SECTION("Monop"){
                compiler_ptr<Instruction> monop(new Monop(Monop_Op::inc, compiler_ptr<Writable>{new Var{"morp"}}));

                std::set<std::string> gen_st{"morp"};
                std::set<std::string> kill_st{};

                REQUIRE(monop->gen() == gen_st);
                REQUIRE(monop->kill() == kill_st);

        }

        SECTION("return"){
                compiler_ptr<Instruction> ret(new Return{8});

                std::set<std::string> gen_st{};
                std::set<std::string> kill_st{};

                for(auto reg : Lang_Constants::caller_saves){
                        gen_st.insert(reg);
                }
                gen_st.insert("rax");

                REQUIRE(ret->gen() == gen_st);
                REQUIRE(ret->kill() == kill_st);
        }

        SECTION("shop"){
                compiler_ptr<Instruction> shop(new Shop{Shop_Op::left_Shift,
                                        compiler_ptr<Writable>{new Writable_Reg{"rax"}},
                                        compiler_ptr<Value_Source>{new Var{"mlorp"}}});

                std::set<std::string> gen_st{"rax", "mlorp"};
                std::set<std::string> kill_st{};

                REQUIRE(shop->gen() == gen_st);
                REQUIRE(shop->kill() == kill_st);
        }

        SECTION("const shop"){
                compiler_ptr<Instruction> shop(new Shop{Shop_Op::left_Shift,
                                        compiler_ptr<Writable>{new Writable_Reg{"rax"}},
                                        compiler_ptr<Value_Source>{new Integer_Literal{"7"}}});

                std::set<std::string> gen_st{"rax"};
                std::set<std::string> kill_st{};

                REQUIRE(shop->gen() == gen_st);
                REQUIRE(shop->kill() == kill_st);
        }

}

TEST_CASE("Label equality"){
        L2_Label hi(":hi");
        L2_Label hi2(":hi");
        L2_Label hlerp(":hlerp");
        REQUIRE(hi2 == hi);
        REQUIRE(hlerp != hi);
}

TEST_CASE("Successor funciton"){
        Function f{L2_Target_Label{"hi"}, 0, 0};

        compiler_ptr<Instruction> store{new Binop(Binop_Op::store,
                                                  compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                  compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

        compiler_ptr<Instruction> store2{new Binop(Binop_Op::store,
                                                   compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                   compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

        compiler_ptr<Instruction> shop(new Shop{Shop_Op::left_Shift,
                                compiler_ptr<Writable>{new Writable_Reg{"rax"}},
                                compiler_ptr<Value_Source>{new Integer_Literal{"7"}}});

        compiler_ptr<Instruction> jump(new Goto{":mlarp"});

        compiler_ptr<Instruction> targ(new L2_Target_Label{":mlarp"});

        compiler_ptr<Instruction> t_targ(new L2_Target_Label{":true"});
        compiler_ptr<Instruction> f_targ(new L2_Target_Label{":false"});
        compiler_ptr<Instruction> cjump(new Cond_Jump(Cmp_Op::equal,
                                                      compiler_ptr<Value_Source>{new Var{"hi"}},
                                                      compiler_ptr<Value_Source>{new Reg{"mlarp"}},
                                                      L2_Label{":true"},
                                                      L2_Label{":false"}));

        SECTION("Successor of store"){
                f.instructions.push_back(store);
                f.instructions.push_back(shop);

                std::stringstream ss{};

                std::stringstream out{};

                shop->dump(out);

                (*f.find_successors(f.instructions.begin())[0])->dump(ss);

                REQUIRE(ss.str() == out.str());
        }

        SECTION("Successor of goto"){
                f.instructions.push_back(store);
                f.instructions.push_back(jump);
                f.instructions.push_back(shop);
                f.instructions.push_back(targ);
                f.instructions.push_back(store2);

                std::vector<Inst_Posn> result{f.instructions.begin() + 3};


                REQUIRE(*f.find_successors(f.instructions.begin() + 1)[0] == *result[0]);
        }


        SECTION("Successor of cjump"){
                f.instructions.push_back(store); // 0
                f.instructions.push_back(jump); // 1
                f.instructions.push_back(shop); // 2
                f.instructions.push_back(t_targ); //3
                f.instructions.push_back(targ); //4
                f.instructions.push_back(cjump); //5
                f.instructions.push_back(f_targ); //6
                f.instructions.push_back(store2);

                std::vector<Inst_Posn> expected{f.instructions.begin() + 3, f.instructions.begin() + 6};

                auto result = f.find_successors(f.instructions.begin() + 5);

                REQUIRE(expected == result);
        }

}

TEST_CASE("Liveness calculation"){
        Function f{L2_Target_Label{"hi"}, 0, 0};

        compiler_ptr<Instruction> store{new Binop(Binop_Op::store,
                                                  compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                  compiler_ptr<Binop_Rhs>{new Writable_Reg("rcx")})};

        compiler_ptr<Instruction> store2{new Binop(Binop_Op::store,
                                                   compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                   compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

        compiler_ptr<Instruction> shop(new Shop{Shop_Op::left_Shift,
                                compiler_ptr<Writable>{new Writable_Reg{"rax"}},
                                compiler_ptr<Value_Source>{new Integer_Literal{"7"}}});

        compiler_ptr<Instruction> jump(new Goto{":mlarp"});

        compiler_ptr<Instruction> targ(new L2_Target_Label{":mlarp"});

        compiler_ptr<Instruction> t_targ(new L2_Target_Label{":true"});
        compiler_ptr<Instruction> f_targ(new L2_Target_Label{":false"});
        compiler_ptr<Instruction> cjump(new Cond_Jump(Cmp_Op::equal,
                                                      compiler_ptr<Value_Source>{new Var{"hi"}},
                                                      compiler_ptr<Value_Source>{new Reg{"mlarp"}},
                                                      L2_Label{":true"},
                                                      L2_Label{":false"}));

        compiler_ptr<Instruction> ret(new Return(0));

        SECTION("Store"){
                f.instructions.push_back(store);
                f.instructions.push_back(ret);

                liveness_sets_t result = {
                        {//in
                                {"rcx", "r12", "r13", "r14", "r15", "rbp", "rbx"},
                                {"rax", "r12", "r13", "r14", "r15", "rbp", "rbx"}
                        },

                        {//out
                                {"rax", "r12", "r13", "r14", "r15", "rbp", "rbx"},
                                {}
                        }
                };

                REQUIRE(f.make_liveness_set() == result);
        }
}
