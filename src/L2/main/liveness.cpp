#include <L2/L2_parser.h>
#include <iostream>

void print_io_set_sexpr(const std::vector<L2::io_set_t> &liveness_set){
        return;
}

int main(int argc, char** argv){
        if(argc != 1){
                std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        }

        print_io_set_sexpr(L2::Function{
                        L2::parse_function_file(
                                std::string{argv[1]})}
                .make_liveness_set());

        return 0;
}
