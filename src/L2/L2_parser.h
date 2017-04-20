#pragma once
#include <L2/AST/L2.h>
#include <deque>
#include <sstream>
#include <exception>
#include <string>
#include <parse_stack.h>

namespace L2{
        Program L2_parse_file (const char *fileName);

        Function parse_function_file (std::string fileName);
}


