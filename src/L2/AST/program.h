#pragma once

#include <L2/AST/labels.h>
#include <vector>
#include <L2/AST/function.h>

namespace L2{
        class Program : public virtual AST_Item{
        public:

                Program() = default;
                Program(Program&& rhs) = default;

                explicit Program(std::string label);

                ~Program() override = default;

                void allocate_registers();

                void dump(std::ostream &out) const override;
                void accept(AST_Item_Visitor &v) override;


                L2_Label entryPointLabel;
                std::vector<compiler_ptr<Function>> functions;

        private:
                static const char* prog_prefix;
                static const std::array<char*, 6> callee_saves;
        };
}
