#pragma once

#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <type_traits>
#include <exception>
#include <unordered_map>

#include <L1/AST/translatable.h>
#include <L1/AST/marker_classes.h>

#include <L1/AST/labels.h>
#include <L1/AST/regs.h>
#include <L1/AST/int_literal.h>

#include <L1/AST/binop.h>
#include <L1/AST/monop.h>
#include <L1/AST/shop.h>

#include <L1/AST/cmp_ops.h>
#include <L1/AST/cond_jump.h>
#include <L1/AST/cmp_store.h>

#include <L1/AST/lea.h>
#include <L1/AST/return.h>
#include <L1/AST/memory_ref.h>
#include <L1/AST/calls.h>
#include <L1/AST/goto.h>
#include <L1/AST/function.h>
#include <L1/AST/program.h>

class NotImplementedException : public std::logic_error
{
public:
NotImplementedException() :
        std::logic_error("Function not implemented. Moron.")
        {};
};
