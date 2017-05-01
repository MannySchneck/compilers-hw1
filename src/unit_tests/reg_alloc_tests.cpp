#include <L2/reg_allocation/interference_graph.h>
#include <iter_enum.h>
#include <iostream>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <catch.hpp>


TEST_CASE("enum iteration works"){
        SECTION("compare by for loop"){
                int i = 0;
                for(auto e : Enum<GPR_Color>()){
                        REQUIRE(toUType(e) == i);
                        i++;
                }
        }
}

TEST_CASE("test boost optional to see if it works..."){
        SECTION("stupid"){
                IG_Node node;
                REQUIRE(!node.get_color());
                REQUIRE(node.get_color() == boost::none);
                node.set_color(GPR_Color::rdi);
                REQUIRE(node.get_color());
                REQUIRE(*node.get_color() == GPR_Color::rdi);
        }
}

TEST_CASE("interference graph generation"){
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

                compiler_ptr<L2::Function> f(new Function());

                f->instructions.push_back(store);
                f->instructions.push_back(ret);


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

                std::vector<std::string> regs;

                adjacency_set_t a_set_result;

                for(auto color : Enum<GPR_Color>{}){
                        neighbor_set_t neighbors;
                        for(auto color2 : Enum<GPR_Color>{}){
                                if(color != color2){
                                        neighbors.insert(IG_Node{GPR_Color_to_string(color2), color2});
                                }
                        }
                        a_set_result[IG_Node{GPR_Color_to_string(color), color}] = neighbors;
                }


                REQUIRE(Interference_Graph(f) == Interference_Graph());
        }
}
