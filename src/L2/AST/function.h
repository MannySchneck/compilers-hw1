#pragma once
#include <L2/AST/translatable.h>
#include <L2/AST/labels.h>
#include <L2/AST/marker_classes.h>
#include <L2/AST/instruction.h>
#include <L2/AST/binop.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <L2/reg_allocation/interference_graph.h>


namespace L2{

        class Interference_Graph;

        // Mr. Soustrup, you make me sad.
        class IG_Node;

        using nodes_set_t = std::map<std::string, compiler_ptr<IG_Node>>;
        using neighbor_set_t = std::set<compiler_ptr<IG_Node>,
                                                     Interference_Graph::Node_Ptr_Cmp>;
        using adjacency_set_t = std::map<std::string,
                                         neighbor_set_t>;


        class Function : public virtual AST_Item{
        public:
                Function();
                Function(L2_Target_Label name, int64_t args, int64_t locals);
                Function(Function&& rhs) = default;

                std::vector<Inst_Posn> find_successors(Inst_Posn);

                void populate_liveness_sets();
                Interference_Graph make_interference_graph();
                void spill_these(std::vector<compiler_ptr<IG_Node>>);
                compiler_ptr<Function> allocate_registers();

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

                std::string find_prefix();
                std::string get_prefix();


        private:
                int prefix_counter;
                boost::optional<std::string> prefix;
        };
}
