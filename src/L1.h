#pragma once

#include <vector>
#include <memory>

namespace L1 {

  struct L1_item {
    std::string labelName;
  };

  struct Function{
    std::string name;
    int64_t arguments;
    int64_t locals;
  };

  struct Program{
    std::string entryPointLabel;
          std::vector<std::unique_ptr<L1::Function>> functions;
  };

} // L1
