#include <catch.hpp>
#include <parser.h>
#include <sstream>
#include <iostream>

using namespace L1;

Program parse(std::string path){
        std::string base{"/home/manny/322/hw/compiler/src/unit_tests/progs/"};
        base.append(path);
        return L1_parse_file(base);
}

TEST_CASE("parsr (I hardly even know her!)"){
        std::stringstream parsed;
        std::stringstream built;
        SECTION("One store"){
                Program p = parse("test1.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst
                        (new Binop(Binop_Op::store,
                                   std::unique_ptr<Binop_Lhs>(new Writable_Reg{"rax"}),
                                   std::unique_ptr<Binop_Rhs>{new Writable_Reg("rdi")}));

                f->instructions.push_back(std::move(inst));

                result.functions.push_back(std::move(f));

                result.translate(built);

                p.translate(parsed);

                REQUIRE(parsed.str() == built.str());
        }

        SECTION("One stored label"){
                Program p = parse("test1.1.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst
                        (new Binop(Binop_Op::store,
                                   std::unique_ptr<Binop_Lhs>(new Memory_Ref{Reg{"rsp"}, -8}),
                                   std::unique_ptr<Binop_Rhs>{new L1_Label(":testFunction_ret")}));

                f->instructions.push_back(std::move(inst));

                result.functions.push_back(std::move(f));

                result.translate(built);

                p.translate(parsed);

                REQUIRE(parsed.str() == built.str());
        }


        SECTION("store w/ mem"){
                Program p = parse("test2.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst
                        (new Binop(Binop_Op::store,
                                   std::unique_ptr<Binop_Lhs>(new Writable_Reg{"rax"}),
                                   std::unique_ptr<Binop_Rhs>{new Memory_Ref(Reg{"rdi"}, 16)}));

                f->instructions.push_back(std::move(inst));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("store w/ mem flip"){
                Program p = parse("test3.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst
                        (new Binop(Binop_Op::store,
                                   std::unique_ptr<Binop_Lhs>(new Memory_Ref(Reg{"rdi"}, 16)),
                                   std::unique_ptr<Binop_Rhs>{new Writable_Reg{"rax"}}));

                f->instructions.push_back(std::move(inst));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("store immediate"){
                Program p = parse("test4.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst
                        (new Binop(Binop_Op::store,
                                   std::unique_ptr<Binop_Lhs>(new Writable_Reg{"rdi"}),
                                   std::unique_ptr<Binop_Rhs>{new Integer_Literal{"5"}}));

                f->instructions.push_back(std::move(inst));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("Store immediate and add"){
                Program p = parse("test5.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst
                        (new Binop(Binop_Op::store,
                                   std::unique_ptr<Binop_Lhs>(new Writable_Reg{"rdi"}),
                                   std::unique_ptr<Binop_Rhs>{new Integer_Literal{"5"}}));
                std::unique_ptr<Instruction> inst2
                        (new Binop(Binop_Op::add_Assign,
                                   std::unique_ptr<Binop_Lhs>(new Writable_Reg{"rdi"}),
                                   std::unique_ptr<Binop_Rhs>{new Writable_Reg{"rsi"}}));


                f->instructions.push_back(std::move(inst));
                f->instructions.push_back(std::move(inst2));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("Store immediate and sub"){
                Program p = parse("test5.5.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst
                        (new Binop(Binop_Op::store,
                                   std::unique_ptr<Binop_Lhs>(new Writable_Reg{"rdi"}),
                                   std::unique_ptr<Binop_Rhs>{new Integer_Literal{"5"}}));
                std::unique_ptr<Instruction> inst2
                        (new Binop(Binop_Op::sub_Assign,
                                   std::unique_ptr<Binop_Lhs>(new Writable_Reg{"rdi"}),
                                   std::unique_ptr<Binop_Rhs>{new Writable_Reg{"rsi"}}));


                f->instructions.push_back(std::move(inst));
                f->instructions.push_back(std::move(inst2));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }


        SECTION("Store immediate and add and shift"){
                Program p = parse("test6.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst
                        (new Binop(Binop_Op::store,
                                   std::unique_ptr<Binop_Lhs>(new Writable_Reg{"rdi"}),
                                   std::unique_ptr<Binop_Rhs>{new Integer_Literal{"5"}}));
                std::unique_ptr<Instruction> inst2
                        (new Binop(Binop_Op::add_Assign,
                                   std::unique_ptr<Binop_Lhs>(new Writable_Reg{"rdi"}),
                                   std::unique_ptr<Binop_Rhs>{new Writable_Reg{"rsi"}}));
                std::unique_ptr<Instruction> inst3
                        (new Shop(Shop_Op::left_Shift,
                                  std::unique_ptr<Writable_Reg>(new Writable_Reg{"rax"}),
                                  std::unique_ptr<Value_Source>{new Writable_Reg{"rcx"}}));


                f->instructions.push_back(std::move(inst));
                f->instructions.push_back(std::move(inst2));
                f->instructions.push_back(std::move(inst3));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("shift immediate"){
                Program p = parse("test7.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst3
                        (new Shop(Shop_Op::left_Shift,
                                  std::unique_ptr<Writable_Reg>(new Writable_Reg{"rax"}),
                                  std::unique_ptr<Value_Source>{new Integer_Literal{"5"}}));


                f->instructions.push_back(std::move(inst3));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("mnop"){
                Program p = parse("test8.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst3
                        (new Monop(Monop_Op::inc,
                                   std::unique_ptr<Writable_Reg>(new Writable_Reg{"rax"})));


                f->instructions.push_back(std::move(inst3));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("with label instr"){
                Program p = parse("test9.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> lab
                        (new L1_Target_Label(":lab"));

                std::unique_ptr<Instruction> inst3
                        (new Monop(Monop_Op::inc,
                                   std::unique_ptr<Writable_Reg>(new Writable_Reg{"rax"})));


                f->instructions.push_back(std::move(lab));
                f->instructions.push_back(std::move(inst3));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("with cjump and goto"){
                Program p = parse("test10.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> lab
                        (new L1_Target_Label(":lab"));

                std::unique_ptr<Instruction> inst3
                        (new Monop(Monop_Op::inc,
                                   std::unique_ptr<Writable_Reg>(new Writable_Reg{"rax"})));

                std::unique_ptr<Value_Source> lhs(new Writable_Reg("r13"));
                std::unique_ptr<Value_Source> rhs(new Reg("rsi"));
                L1_Label true_target(":true");
                L1_Label false_target(":false");

                std::unique_ptr<Instruction> inst2{new Cond_Jump{Cmp_Op::equal,
                                        std::move(lhs),
                                        std::move(rhs),
                                        true_target,
                                        false_target}};

                std::unique_ptr<Instruction> gone{new Goto{":hi"}};



                f->instructions.push_back(std::move(lab));
                f->instructions.push_back(std::move(inst3));
                f->instructions.push_back(std::move(inst2));
                f->instructions.push_back(std::move(gone));
                f->instructions.push_back(std::unique_ptr<Instruction>(new
                                                                       Return(8)));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("call insstructions"){
                Program p = parse("test11.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> lab
                        (new L1_Target_Label(":lab"));

                std::unique_ptr<Instruction> inst3
                        (new Monop(Monop_Op::inc,
                                   std::unique_ptr<Writable_Reg>(new Writable_Reg{"rax"})));

                std::unique_ptr<Value_Source> lhs(new Writable_Reg("r13"));
                std::unique_ptr<Value_Source> rhs(new Reg("rsi"));
                L1_Label true_target(":true");
                L1_Label false_target(":false");

                std::unique_ptr<Instruction> inst2{new Cond_Jump{Cmp_Op::equal,
                                        std::move(lhs),
                                        std::move(rhs),
                                        true_target,
                                        false_target}};

                std::unique_ptr<Instruction> gone{new Goto{":hi"}};

                std::unique_ptr<Instruction> a_call{new Call{std::unique_ptr<Writable_Reg>{
                                        new Writable_Reg{"rax"}}, 1}};

                std::unique_ptr<Instruction> b_call{new Call{std::unique_ptr<L1_Label>{
                                        new L1_Label{":lab"}}, 2}};


                f->instructions.push_back(std::move(lab));
                f->instructions.push_back(std::move(inst3));
                f->instructions.push_back(std::move(inst2));
                f->instructions.push_back(std::move(gone));
                f->instructions.push_back(std::move(a_call));
                f->instructions.push_back(std::move(b_call));
                f->instructions.push_back(std::unique_ptr<Instruction>(new
                                                                       Return(8)));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

        SECTION("lea"){
                Program p = parse("test13.L1");


                Program result{};
                result.entryPointLabel = {":go"};

                std::unique_ptr<Function> f(new Function({":go"}, 0, 1));
                std::unique_ptr<Instruction> inst3
                        (new LEA(Writable_Reg{"rax"},
                                 Writable_Reg{"r13"},
                                 Writable_Reg{"r15"},
                                 2));


                f->instructions.push_back(std::move(inst3));

                result.functions.push_back(std::move(f));

                p.translate(parsed);
                result.translate(built);


                REQUIRE(parsed.str() == built.str());
        }

}
