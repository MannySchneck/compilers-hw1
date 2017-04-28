#include <L2/L2_parser.h>
#include <iostream>
#include <iterator>


using liveness_sets_t =  std::pair<std::vector<L2::io_set_t>, std::vector<L2::io_set_t>>;

void print_io_sets(const std::vector<L2::io_set_t> &sets, std::ostream& out){
        for(auto s : sets){
                out << "(";
                for(auto elt_it = s.begin(); elt_it != s.end(); elt_it++){
                        out << *elt_it;
                        if(std::next(elt_it) != s.end()) out << " ";
                }
                out << ")" << std::endl;
        }
}


void print_liveness_sexpr(const liveness_sets_t  &liveness_sets, std::ostream& out){
        out << "(" << std::endl;

        out << "(in" << std::endl;
        print_io_sets(liveness_sets.first, out);
        out << ")\n" << std::endl;

        out << "(out" << std::endl;
        print_io_sets(liveness_sets.second, out);
        out << ")\n" << std::endl;

        out << ")" << std::endl;
}

int main(int argc, char** argv){
        if(argc > 2){
                std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        }

        auto f = L2::parse_function_file(std::string{argv[1]});
        f.populate_liveness_sets();

        print_liveness_sexpr(f.make_liveness_sets(), std::cout);

        return 0;
}
