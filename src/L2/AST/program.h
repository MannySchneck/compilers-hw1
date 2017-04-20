#pragma once

#include <L2/AST/labels.h>
#include <vector>
#include <L2/AST/function.h>

namespace L2{
        class Program : public virtual Translatable{
        public:

                Program() = default;
                Program(Program&& rhs) = default;

                Program(std::string label);

                ~Program() override = default;

                void translate(std::ostream&) const override;
                void dump(std::ostream &out) const override;

                L2_Label entryPointLabel;
                std::vector<std::unique_ptr<Function>> functions;

        private:
                static const char* prog_prefix;
                static const std::array<char*, 6> callee_saves;
        };
}
