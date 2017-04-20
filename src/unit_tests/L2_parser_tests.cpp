#include <catch.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <L2/AST/L2.h>
#include <L2_parser.h>

std::string make_test_name(int i){
        std::stringstream name;
        name << "/home/manny/322/hw/schneck-compiler/src/unit_tests/L2_progs/test" << i << ".L2f";

        return name.str();
}

std::string slurp_fun(std::ifstream& in, int i){
        std::stringstream ss;
        in.open(make_test_name(i), std::ios::in);
        ss << in.rdbuf();
        in.close();
        return ss.str();
}

TEST_CASE("Function_parser matches Input file"){
        std::stringstream parsed;
        std::ifstream L2f;

        for(int i =1; i <= 20; i++){
                SECTION("Parsed matches input"){
                        std::string test_function = slurp_fun(L2f, i);

                        L2::Function parsed_fun = L2::parse_function_file(make_test_name(i));

                        parsed_fun.dump(parsed);

                        REQUIRE(parsed.str() == test_function);
                }

        }
}
