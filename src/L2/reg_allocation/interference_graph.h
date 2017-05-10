#pragma once

#include <iterator>
#include <map>
#include <set>
#include <type_traits>
#include <boost/optional.hpp>
#include <ostream>
#include <L2/AST/lang_constants.h>
#include <L2/AST/ast_item.h>
#include <L2/AST/instruction.h>
// Takes liveness sets and generates an interference graph.


namespace L2{

        class Function;

        enum class GPR_Color{
                rdi,
                rsi,
                rdx,
                rcx,
                r8,
                r9,
                rax,
                r10,
                r11,
                r12,
                r13,
                r14,
                r15,
                rbp,
                rbx,
                end,
                begin = rdi,
                };

        GPR_Color string_to_GPRcolor(std::string reg);

///////////////////////////////////////////////////////////////////////////////
//                                  IG node                                  //
///////////////////////////////////////////////////////////////////////////////


        class IG_Node{
        public:
                IG_Node();
                IG_Node(std::string);
                IG_Node(std::string, GPR_Color);

                std::string get_name() const;
                void set_color(GPR_Color c);
                boost::optional<GPR_Color> get_color() const;

                bool operator==(const IG_Node &rhs) const{
                        return this->L2_ID == rhs.L2_ID
                                && this->color == rhs.color;
                }

                bool operator<(const IG_Node &rhs) const{
                        return this->L2_ID < rhs.L2_ID;
                }


                friend std::hash<IG_Node>;
                friend std::ostream& operator<<(std::ostream& out, IG_Node n);
                friend bool operator==(const compiler_ptr<IG_Node>& node1,
                                       const compiler_ptr<IG_Node>& node2);


                boost::optional<GPR_Color> color;
        private:
                std::string L2_ID;
        };


        std::ostream& operator<<(std::ostream& out, IG_Node n);

        bool operator==(const compiler_ptr<IG_Node>& node1,
                        const compiler_ptr<IG_Node>& node2);
}

namespace std{
        template<>
        struct hash<L2::IG_Node>{
                std::size_t operator()(const L2::IG_Node & n) const{
                        return (std::hash<std::string>()(n.L2_ID) ^
                                (n.color ?
                                 std::hash<std::underlying_type_t<L2::GPR_Color>>()
                                 (static_cast<std::underlying_type_t<L2::GPR_Color>>(*n.color)) :
                                 0)); //XXX: idk what to do here...
                }
        };
}

namespace L2{

///////////////////////////////////////////////////////////////////////////////
//                             interference graph                            //
///////////////////////////////////////////////////////////////////////////////


        std::ostream& operator<<(std::ostream& out, GPR_Color c);

        std::ostream& operator<<(std::ostream& out, const std::shared_ptr<IG_Node>& ptr);

        class Interference_Graph{
        public:

                // References to preexisting nodes (neighborship for example),
                // is handled by only giving out pointers.
                // Canonical "node set" avoids duplication of color info

                // Something something another layer of indirection

                class Node_Ptr_Cmp{
                public:
                        bool operator()(const compiler_ptr<IG_Node>& lhs,
                                   const compiler_ptr<IG_Node>& rhs){
                                return lhs->get_name() < rhs->get_name();
                        }
                };

                using nodes_set_t = std::map<std::string, compiler_ptr<IG_Node>>;
                using neighbor_set_t = std::set<compiler_ptr<IG_Node>,
                                                Node_Ptr_Cmp>;
                using adjacency_set_t = std::map<std::string,
                                                 neighbor_set_t>;

                Interference_Graph(compiler_ptr<Function> f);
                Interference_Graph(Function* f);
                Interference_Graph();

                bool operator==(const Interference_Graph &rhs) const{
                        return this->adjacency_set == rhs.adjacency_set;
                }

                friend std::ostream& operator<<(std::ostream&, const Interference_Graph&);


                // Methods to do things:

                // Adds bidirectional edge
                void add_edge(std::string, std::string);

                // Only callable from constructor
                void connect_registers();

                // TODO: methods to query the finished graph
                std::unordered_map<std::string, std::string> get_reg_map();
                std::set<GPR_Color> get_neighbor_colors(std::string id);


                void draw_intra_set_edges(const io_set_t &st);

                std::vector<compiler_ptr<IG_Node>> attempt_coloring();

                ////////////////////
                //data
                ////////////////////
                nodes_set_t nodes_set;
                adjacency_set_t adjacency_set;

        };


        std::ostream& operator<<(std::ostream& out, const Interference_Graph& ig);

        using neighbor_set_t = Interference_Graph::neighbor_set_t;
        using adjacency_set_t = Interference_Graph::adjacency_set_t;

        std::string GPR_Color_to_string(GPR_Color c);
}
