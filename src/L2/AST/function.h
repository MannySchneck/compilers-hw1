#pragma once
#include <L2/AST/translatable.h>
#include <L2/AST/labels.h>
#include <L2/AST/marker_classes.h>
#include <L2/AST/instruction.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>


namespace L2{

        // Mr. Soustrup, you make me sad.
        class IG_Node;

        using neighbor_set_t = std::set<IG_Node>;
        using adjacency_set_t = std::map<IG_Node, neighbor_set_t>;

        class Function : public virtual AST_Item{
        public:
                Function() = default;
                Function(L2_Target_Label name, int64_t args, int64_t locals);
                Function(Function&& rhs) = default;

                std::vector<Inst_Posn> find_successors(Inst_Posn);

                void populate_liveness_sets();
                adjacency_set_t populate_interference_graph(adjacency_set_t adjacency_set);

                liveness_sets_t make_liveness_sets();

                void dump(std::ostream &out) const override;

                int64_t stack_args() const;
                int64_t stack_shift() const;
                void expand_stack(std::ostream&out) const;
                void shrink_stack(std::ostream&out) const;

                L2_Target_Label name;
                int64_t arguments;
                int64_t locals;
                std::vector<compiler_ptr<Instruction>> instructions;
        };
}
