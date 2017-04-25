#include <catch.hpp>
#include <L2/AST/L2.h>
#include <unordered_set>
#include <string>

using namespace L2;

TEST_CASE("Correct gen/kill "){

        compiler_ptr<Instruction> store{new Binop(Binop_Op::store,
                                                  compiler_ptr<Binop_Lhs>{new Writable_Reg("rax")},
                                                  compiler_ptr<Binop_Rhs>{new Writable_Reg("rbx")})};

        std::unordered_set<std::string> gen_st{"rbx"};
        std::unordered_set<std::string> kill_st{"rax"};

        REQUIRE(store->gen() == gen_st);
        REQUIRE(store->kill() == kill_st);
}
