#include <catch.hpp>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <L2/AST/L2.h>
#include <L2_parser.h>

std::string make_test_name(int i){
        std::stringstream name;
        name << "/home/manny/322/hw/compiler/src/unit_tests/L2_progs/test" << i << ".L2f";

        return name.str();
}

std::string make_out_name(int i){
        std::stringstream name;
        name << "/home/manny/322/hw/compiler/src/unit_tests/L2_progs/test" << i << ".L2f.out";

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
        std::ifstream L2f;
        std::ofstream L2f_out;

        SECTION("Parsed matches input"){
                for(int i =1; i <= 20; i++){
                        std::string test_function = slurp_fun(L2f, i);
                        std::stringstream parsed{};


                        L2::Function parsed_fun =
                                L2::parse_function_file(make_test_name(i));

                        parsed_fun.dump(parsed);

                        L2f_out.open(make_out_name(i));
                        L2f_out << parsed.str();
                        L2f_out.close();

                        REQUIRE(parsed.str() == test_function);
                }
        }
}
