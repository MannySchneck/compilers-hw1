#include <L2/reg_allocation/interference_graph.h>
#include <ostream>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <iter_enum.h>
#include <algorithm>

using namespace L2;

///////////////////////////////////////////////////////////////////////////////
//                             interference graph                            //
///////////////////////////////////////////////////////////////////////////////
Interference_Graph::Interference_Graph() = default;

Interference_Graph::Interference_Graph(compiler_ptr<Function> f){
        adjacency_set = connect_registers();

        f->populate_liveness_sets();
        adjacency_set = f->populate_interference_graph(adjacency_set);
}

Interference_Graph::Interference_Graph(adjacency_set_t adj_set) :
        adjacency_set(adj_set)
{}

adjacency_set_t Interference_Graph::connect_registers(){
        adjacency_set_t a_set;
        for(auto color : Enum<GPR_Color>{}){
                neighbor_set_t neighbors;
                for(auto color2 : Enum<GPR_Color>{}){
                        if(color != color2){
                                neighbors.insert(IG_Node{GPR_Color_to_string(color2), color2});
                        }
                }
                a_set[IG_Node{GPR_Color_to_string(color), color}] = neighbors;
        }
        return a_set;
}

bool Interference_Graph::attempt_coloring(){
        using node_edges_pair_t = std::pair<IG_Node, neighbor_set_t>;

        std::vector<node_edges_pair_t> node_stack;

        for(auto node_edges_pair : adjacency_set){
                node_stack.push_back(node_edges_pair);
        }
}

std::ostream& L2::operator<<(std::ostream& out, const Interference_Graph& ig){
        out << ig.adjacency_set;
        return out;
}

///////////////////////////////////////////////////////////////////////////////
//                                  ig node                                  //
///////////////////////////////////////////////////////////////////////////////
IG_Node::IG_Node() = default;

IG_Node::IG_Node(std::string L2_ID) :
        L2_ID(L2_ID){}

IG_Node::IG_Node(std::string L2_ID, GPR_Color color) :
        L2_ID(L2_ID),
        color(color){}

boost::optional<GPR_Color> IG_Node::get_color(){
        return color;
}

void IG_Node::set_color(GPR_Color c){
        color = c;
}

std::string IG_Node::get_name(){
        return L2_ID;
}




std::string L2::GPR_Color_to_string(GPR_Color c){
        static const std::unordered_map<GPR_Color, std::string> color_map{
                {GPR_Color::rdi, "rdi"},
                {GPR_Color::rsi, "rsi"},
                {GPR_Color::rdx, "rdx"},
                {GPR_Color::rcx, "rcx"},
                {GPR_Color::r8, "r8"},
                {GPR_Color::r9, "r9"},
                {GPR_Color::rax, "rax"},
                {GPR_Color::r10, "r10"},
                {GPR_Color::r11, "r11"},
                {GPR_Color::r12, "r12"},
                {GPR_Color::r13, "r13"},
                {GPR_Color::r14, "r14"},
                {GPR_Color::r15, "r15"},
                {GPR_Color::rbp, "rbp"},
                {GPR_Color::rbx, "rbx"}};

        return color_map.at(c);
}

GPR_Color streing_to_GPRcolor(std::string reg){
        static const std::unordered_map<std::string, GPR_Color> color_map{
                {"rdi", GPR_Color::rdi},
                {"rsi", GPR_Color::rsi},
                {"rdx", GPR_Color::rdx},
                {"rcx", GPR_Color::rcx},
                {"r8", GPR_Color::r8},
                {"r9", GPR_Color::r9},
                {"rax", GPR_Color::rax},
                {"r10", GPR_Color::r10},
                {"r11", GPR_Color::r11},
                {"r12", GPR_Color::r12},
                {"r13", GPR_Color::r13},
                {"r14", GPR_Color::r14},
                {"r15", GPR_Color::r15},
                {"rbp", GPR_Color::rbp},
                {"rbx", GPR_Color::rbx}};
        return color_map.at(reg);
}


std::ostream& L2::operator<<(std::ostream& out, GPR_Color c){
        out << GPR_Color_to_string(c);
        return out;
}

std::ostream& L2::operator<<(std::ostream& out, IG_Node n){
        out << "(IG_Node: ";
        out << n.L2_ID << " ";
        if(n.color){
                out << GPR_Color_to_string(*n.color);
        } else{
                out << "nullopt";
        }
        out << ")";
        return out;
}
