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

#include <L2/AST/ast_item.h>
#include <L2/AST/translatable.h>
#include <L2/AST/marker_classes.h>

#include <L2/AST/labels.h>
#include <L2/AST/regs.h>
#include <L2/AST/int_literal.h>
#include <L2/AST/var.h>
#include <L2/AST/memory_ref.h>
#include <L2/AST/stack_arg.h>

#include <L2/AST/binop.h>
#include <L2/AST/monop.h>
#include <L2/AST/shop.h>

#include <L2/AST/cmp_ops.h>
#include <L2/AST/cond_jump.h>
#include <L2/AST/cmp_store.h>

#include <L2/AST/lea.h>
#include <L2/AST/return.h>
#include <L2/AST/calls.h>
#include <L2/AST/goto.h>
#include <L2/AST/function.h>
#include <L2/AST/program.h>

class NotImplementedException : public std::logic_error
{
public:
NotImplementedException() :
        std::logic_error("Function not implemented. Moron.")
        {};
};
