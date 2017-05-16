#include <L2/AST/L2.h>
#include <L2/L2_parser.h>

int main(int argc, char* argv[]){

        if(argc < 2){
                std::cerr << "USAGE: " << argv[0] << " <source> [-v]" << std::endl;
                std::cerr << "The -v flag is not implemented." << std::endl;
                return 1;
        }

        L2::Program p(L2::L2_parse_file(std::string(argv[1])));

        fstream outfile("prog.L1", ios_base::out);

        p.translate(outfile);
}
