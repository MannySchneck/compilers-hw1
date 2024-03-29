#pragma once

#include <L2/AST/marker_classes.h>
#include <L2/AST/instruction.h>
#include <L2/AST/labels.h>

using namespace L2;

class Jump{
public:
        Inst_Posn find_target(std::vector<Inst_Ptr>::iterator begin,
                              std::vector<Inst_Ptr>::iterator end,
                              const L2_Label &lab);
};
