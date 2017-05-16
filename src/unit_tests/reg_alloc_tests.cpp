#include <L2/reg_allocation/interference_graph.h>
#include <L2/AST/L2.h>
#include <iter_enum.h>
#include <iostream>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <catch.hpp>
#include <algorithm>
#include <sstream>
#include <prettyprint.hpp>
#include <L2/L2_parser.h>

// TEST_CASE("test boost optional to see if it works..."){
//         SECTION("stupid"){
//                 IG_Node node;
//                 REQUIRE(!node.get_color());
//                 REQUIRE(node.get_color() == boost::none);
//                 node.set_color(GPR_Color::rdi);
//                 REQUIRE(node.get_color());
//                 REQUIRE(*node.get_color() == GPR_Color::rdi);
//         }
// }



TEST_CASE("interference graph generation"){

        std::string test_prefix = "/home/manny/322/hw/compiler/src/unit_tests/alloc_test_funs/";

        compiler_ptr<Instruction>
                store{new Binop(Binop_Op::store,
                                compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                compiler_ptr<Binop_Rhs>{new Writable_Reg("rcx")})};

        compiler_ptr<Instruction>
                store2{new Binop(Binop_Op::store,
                                 compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                 compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

        compiler_ptr<Instruction>
                storevar{new Binop(Binop_Op::store,
                                   compiler_ptr<Binop_Lhs> {new Writable_Reg("rax")},
                                   compiler_ptr<Binop_Rhs> {new Writable_Reg("blerp")})};

        compiler_ptr<Instruction>
                rdi2foo{new Binop(Binop_Op::store,
                                  compiler_ptr<Binop_Lhs>{new Writable_Reg("foo")},
                                  compiler_ptr<Binop_Rhs>{new Writable_Reg("rdi")})};


        compiler_ptr<Instruction>
                storevar2{new Binop(Binop_Op::store,
                                    compiler_ptr<Binop_Lhs> {new Var("foo")},
                                    compiler_ptr<Binop_Rhs> {new Writable_Reg("rax")})};

        compiler_ptr<Instruction>
                foo_to_ret{new Binop(Binop_Op::store,
                                     compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                     compiler_ptr<Binop_Rhs>{new Writable_Reg("foo")})};

        compiler_ptr<Instruction> shop(new Shop{Shop_Op::left_Shift,
                                compiler_ptr<Writable>{new Writable_Reg{"rax"}},
                                compiler_ptr<Value_Source>{new Integer_Literal{"7"}}});

        compiler_ptr<Instruction> jump(new Goto{":mlarp"});

        compiler_ptr<Instruction> targ(new L2_Target_Label{":mlarp"});

        compiler_ptr<Instruction> t_targ(new L2_Target_Label{":true"});
        compiler_ptr<Instruction> f_targ(new L2_Target_Label{":false"});
        compiler_ptr<Instruction>
                cjump(new Cond_Jump(Cmp_Op::equal,
                                    compiler_ptr<Value_Source>{new Var{"hi"}},
                                    compiler_ptr<Value_Source>{new Reg{"mlarp"}},
                                                      L2_Label{":true"},
                                                      L2_Label{":false"}));

        compiler_ptr<Instruction> ret(new Return(0));


        Interference_Graph ig;


        compiler_ptr<L2::Function> f(new Function());

        SECTION("Sanity check"){
                REQUIRE(ig == ig);
        }

        SECTION("Store"){
                /*
                  (:foo
                   0 0
                   (rax <- rcx)
                   (return)
                   )
                 */
                /*
                  expected result
                 */

                f->instructions.push_back(store);
                f->instructions.push_back(ret);
                f->populate_liveness_sets();

                Interference_Graph result{f};

                adjacency_set_t diff;

                //std::cout << ig;

                REQUIRE(result == ig);
        }

        SECTION("Store var"){
                /*
                  (:foo
                  0 0
                  (rax <- blerp)
                  (return)
                  )
                */

                /*
                  expected after trivial allocation:
                  (:foo
                  0 0
                  (rax <- rdi)
                  )
                 */

                f->instructions.push_back(storevar);
                f->instructions.push_back(ret);
                f->populate_liveness_sets();


                for(auto reg : Lang_Constants::callee_saves){
                        ig.add_edge("blerp", reg);
                }

                REQUIRE(Interference_Graph{f} == ig);
        }

        // SECTION("get colors"){

        //         Interference_Graph graph;

        //         std::set<GPR_Color> expected_colors;

        //         for(auto color : Enum<GPR_Color>()){
        //                 if(color != GPR_Color::rax)
        //                         expected_colors.insert(color);
        //         }

        //         // std::cout << "Here it is!" << std::endl;
        //         // for(auto color : expected_colors){
        //         //         std::cout << (int) color;
        //         // }

        //         // std::cout << std::endl;
        //         // std::cout << std::endl;
        //         // for(auto color : graph.get_neighbor_colors(IG_Node{"rax"})){
        //         //         std::cout << (int) color;
        //         // }

        //         REQUIRE(graph.get_neighbor_colors("rax") == expected_colors);
        // }

        SECTION("Reg allocation"){
                SECTION("Trivial reg alloc"){
                        /*
                          (:f
                          0 0
                          (r8 <- rdi)
                          (r8 += 3)
                          (r8 -= rdi)
                          (rax <- r8)
                          (ret)
                          )
                        */
                        compiler_ptr<Instruction>
                                rdi2r8{new Binop(Binop_Op::store,
                                                 compiler_ptr<Binop_Lhs> {new Writable_Reg("r8")},
                                                 compiler_ptr<Binop_Rhs> {new Writable_Reg("rdi")})};
                        compiler_ptr<Instruction>
                                r82rax{new Binop(Binop_Op::store,
                                                 compiler_ptr<Binop_Lhs> {new Writable_Reg("rax")},
                                                 compiler_ptr<Binop_Rhs> {new Writable_Reg("r8")})};


                        compiler_ptr<Instruction>
                                sub_rdi{new Binop(Binop_Op::sub_Assign,
                                                  compiler_ptr<Binop_Lhs> {new Writable_Reg("r8")},
                                                  compiler_ptr<Binop_Rhs> {new Writable_Reg("rdi")})};

                        compiler_ptr<Instruction>
                                add3{new Binop(Binop_Op::add_Assign,
                                               compiler_ptr<Binop_Lhs> {new Writable_Reg("r8")},
                                               compiler_ptr<Binop_Rhs> {new Integer_Literal("3")})};


                        f->instructions.push_back(rdi2r8);
                        f->instructions.push_back(add3);
                        f->instructions.push_back(sub_rdi);
                        f->instructions.push_back(r82rax);
                        f->instructions.push_back(ret);
                        f->name = L2_Target_Label(":f");

                        std::stringstream ss;

                        auto new_f = f->allocate_registers();

                        new_f->dump(ss);


                        REQUIRE(ss.str() ==
                                "(:f\n"           \
                                "0 0\n"           \
                                "(r8 <- rdi)\n"  \
                                "(r8 += 3)\n"  \
                                "(r8 -= rdi)\n"  \
                                "(rax <- r8)\n"  \
                                "(return)\n"        \
                                ")\n");
                }


                SECTION("dumb reg alloc"){
                        /*
                          (:f
                          0 0
                          (foo <- rdi) // stupid....
                          (rax <- foo)
                          (ret)
                          )
                        */
                        f->instructions.push_back(rdi2foo);
                        f->instructions.push_back(foo_to_ret);
                        f->instructions.push_back(ret);
                        f->name = L2_Target_Label(":f");

                        Function fun = parse_function_file(test_prefix.append("test1.l2f"));

                        compiler_ptr<Function> fun_ptr{new Function(std::move(fun))};

                        auto newF = fun_ptr->allocate_registers();

                        std::stringstream ss;

                        newF->dump(ss);

                        REQUIRE(ss.str() ==
                                "(:t1f\n"           \
                                "0 0\n"           \
                                "(rdi <- rdi)\n"  \
                                "(rax <- rdi)\n"  \
                                "(return)\n"      \
                                ")\n");
                }

                SECTION("Simple reg alloc"){

                        Function fun = parse_function_file(test_prefix.append("test2.l2f"));

                        compiler_ptr<Function> fun_ptr{new Function(std::move(fun))};

                        auto newF = fun_ptr->allocate_registers();

                        std::stringstream ss;

                        newF->dump(ss);

                        REQUIRE(ss.str() ==
                                "(:t2f\n"           \
                                "0 0\n"           \
                                "(rdi <- rsi)\n"  \
                                "(rdi <- rdi)\n"  \
                                "(rdi <- rdi)\n"  \
                                "(rax <- rdi)\n"  \
                                "(return)\n"      \
                                ")\n");
                }


                SECTION("Siple conflicting reg alloc"){

                                Function fun = parse_function_file(test_prefix.append("test3.l2f"));

                                compiler_ptr<Function> fun_ptr{new Function(std::move(fun))};

                                auto newF = fun_ptr->allocate_registers();

                                std::stringstream ss;

                                newF->dump(ss);

                                REQUIRE(ss.str() ==
                                        "(:t3f\n" \
                                        "0 0\n"   \
                                        "(rdi <- rdi)\n"  \
                                        "(rsi <- rsi)\n"  \
                                        "(rdi += rsi)\n"  \
                                        "(rax <- rdi)\n"  \
                                        "(return)"      \
                                        "\n)\n");
                }

        }

}

TEST_CASE("test getting function prefix"){

        std::string test_prefix = "/home/manny/322/hw/compiler/src/unit_tests/alloc_test_funs/";

        SECTION("trivial"){
                Function fun = parse_function_file(test_prefix.append("test1.l2f"));

                REQUIRE(fun.find_prefix() == "z");
        }


        SECTION("basic"){
                Function funz = parse_function_file(test_prefix.append("test5.l2f"));

                REQUIRE(funz.find_prefix() == "za");
        }

        SECTION("need to cycle"){
                Function funz = parse_function_file(test_prefix.append("test6.l2f"));

                REQUIRE(funz.find_prefix() == "zc");
        }

        SECTION("need to cycle twice"){
                Function funz = parse_function_file(test_prefix.append("test7.l2f"));

                REQUIRE(funz.find_prefix() == "zd");
        }
}

TEST_CASE("Instruction visitor"){
        SECTION("on a binop"){
                compiler_ptr<Instruction>
                        foo_to_ret{new Binop(Binop_Op::store,
                                             compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                             compiler_ptr<Binop_Rhs>{new Writable_Reg("foo")})};

                Get_Ids_Visitor v;

                foo_to_ret->accept(v);

                std::vector<std::string> expected = {"rax", "foo"};

                REQUIRE(v.result == expected);
        }
}


TEST_CASE("Spilling"){

        std::string test_prefix = "/home/manny/322/hw/compiler/src/unit_tests/alloc_test_funs/";


        SECTION("Allocation on 16 live vars"){
                Function fun = parse_function_file(test_prefix.append("test4.l2f"));

                compiler_ptr<Function> fun_ptr{new Function(std::move(fun))};

                auto newF = fun_ptr->allocate_registers();

                std::stringstream ss;

                newF->dump(ss);

                REQUIRE(ss.str() ==
                        "(:t4f\n"
                        "0 0\n"
                        "(rax += 3)\n"
                        "(rbx += 3)\n"
                        "(rbp += 3)\n"
                        "(r15 += 3)\n"
                        "(r14 += 3)\n"
                        "(r13 += 3)\n"
                        "(r12 += 3)\n"
                        "(r11 += 3)\n"
                        "(r10 += 3)\n"
                        "(rax <- (mem rsp 8))\n"
                        "(rax += 3)\n"
                        "((mem rsp 8) <- rax)\n"
                        "(r9 += 3)\n"
                        "(r8 += 3)\n"
                        "(rcx += 3)\n"
                        "(rdx += 3)\n"
                        "(rsi += 3)\n"
                        "(rdi += 3)\n"
                        "(rax <- rdi)\n"
                        ")\n");
        }
}
