#include <L2/reg_allocation/interference_graph.h>
#include <iter_enum.h>
#include <iostream>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <catch.hpp>
#include <algorithm>
#include <prettyprint.hpp>

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

        compiler_ptr<Instruction> store{new Binop(Binop_Op::store,
                                                  compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                  compiler_ptr<Binop_Rhs>{new Writable_Reg("rcx")})};

        compiler_ptr<Instruction> store2{new Binop(Binop_Op::store,
                                                   compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                   compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

        compiler_ptr<Instruction> storevar{new Binop(Binop_Op::store,
                                                   compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                   compiler_ptr<Binop_Rhs>{new Writable_Reg("blerp")})};


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


        adjacency_set_t regs_connected;

        for(auto color : Enum<GPR_Color>{}){
                neighbor_set_t neighbors;
                for(auto color2 : Enum<GPR_Color>{}){
                        if(color != color2){
                                neighbors.insert(IG_Node{GPR_Color_to_string(color2), color2});
                        }
                }
                regs_connected[IG_Node{GPR_Color_to_string(color), color}] = neighbors;
        }


        compiler_ptr<L2::Function> f(new Function());

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

                Interference_Graph exp_result{regs_connected};
                Interference_Graph result{f};

                adjacency_set_t diff;

                //std::cout << diff;

                REQUIRE(result == exp_result);
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

                //std::cout << f->make_liveness_sets();

                regs_connected[IG_Node{"blerp"}].insert(Lang_Constants::callee_saves.begin(),
                                                        Lang_Constants::callee_saves.end());

                REQUIRE(Interference_Graph{f} == Interference_Graph{regs_connected});
        }
}
