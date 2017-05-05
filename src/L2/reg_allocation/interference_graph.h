#pragma once

#include <iterator>
#include <map>
#include <set>
#include <type_traits>
#include <boost/optional.hpp>
#include <ostream>
#include <L2/AST/lang_constants.h>
#include <L2/AST/L2.h>
// Takes liveness sets and generates an interference graph.

namespace L2{

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

                boost::optional<GPR_Color> get_color();
                void set_color(GPR_Color c);

                bool operator==(const IG_Node &rhs) const{
                        return this->L2_ID == rhs.L2_ID
                                && this->color == rhs.color;
                }

                bool operator<(const IG_Node &rhs) const{
                        return this->L2_ID < rhs.L2_ID;
                }

                std::string get_name();

                friend std::hash<IG_Node>;
                friend std::ostream& operator<<(std::ostream& out, IG_Node n);
        private:
                std::string L2_ID;
                boost::optional<GPR_Color> color;
        };
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

        class Interference_Graph{
        public:
                using neighbor_set_t = std::set<IG_Node>;
                using adjacency_set_t = std::map<IG_Node, neighbor_set_t>;
                Interference_Graph(compiler_ptr<Function> f);
                Interference_Graph(adjacency_set_t adj_set);
                Interference_Graph();

                bool operator==(const Interference_Graph &rhs) const{
                        return this->adjacency_set == rhs.adjacency_set;
                }

                friend std::ostream& operator<<(std::ostream&, const Interference_Graph&);

                adjacency_set_t adjacency_set;
                adjacency_set_t connect_registers();
        };

        using neighbor_set_t = Interference_Graph::neighbor_set_t;
        using adjacency_set_t = Interference_Graph::adjacency_set_t;

        std::string GPR_Color_to_string(GPR_Color c);

}
