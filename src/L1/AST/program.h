#pragma once

#include <L1/AST/labels.h>
#include <vector>
#include <L1/AST/function.h>

namespace L1{
        class Program : public virtual Translatable{
        public:

                Program() = default;
                Program(Program&& rhs) = default;

                Program(std::string label);

                ~Program() override = default;

                void translate(std::ostream&) const override;

                L1_Label entryPointLabel;
                std::vector<std::unique_ptr<Function>> functions;

        private:
                static const char* prog_prefix;
                static const std::array<char*, 6> callee_saves;
        };
}
