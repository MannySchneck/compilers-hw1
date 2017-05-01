#include <catch.hpp>
#include <L2/reg_allocation/interference_graph.h>
#include <iter_enum.h>
#include <iostream>


TEST_CASE("enum iteration works"){
        SECTION("compare by for loop"){
                int i = 0;
                for(auto e : Enum<GPR_Color>()){
                        REQUIRE(toUType(e) == i);
                        i++;
                }
        }
}
