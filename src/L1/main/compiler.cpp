#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
#include <fstream>
#include <parser.h>

using namespace std;

int main(
        int argc,
        char **argv
        ){
        bool verbose;

        /* Check the input.
         */
        if( argc < 2 ) {
                std::cerr << "Usage: " << argv[ 0 ] << " SOURCE [-v]" << std::endl;
                return 1;
        }
        int32_t opt;
        while ((opt = getopt(argc, argv, "v")) != -1) {
                switch (opt){
                case 'v':
                        verbose = true;
                        break ;

                default:
                        std::cerr << "Usage: " << argv[ 0 ] << "[-v] SOURCE" << std::endl;
                        return 1;
                }
        }

        /* Parse the L1 program.
         */
        L1::Program p(L1::L1_parse_file(std::string(argv[optind])));


        /* Generate x86_64 code
         */

        fstream outfile("prog.S", outfile.out);
        p.translate(outfile);

        return 0;
}
