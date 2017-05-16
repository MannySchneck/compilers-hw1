#include <L2/reg_allocation/interference_graph.h>
#include <ostream>
#include <boost/optional.hpp>
#include <boost/optional/optional_io.hpp>
#include <iter_enum.h>
#include <algorithm>
#include <L2/AST/function.h>
#include <cassert>
#include <prettyprint.hpp>
#include <iostream>

using namespace L2;



GPR_Color L2::string_to_GPRcolor(std::string reg){
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

///////////////////////////////////////////////////////////////////////////////
//                             interference graph                            //
///////////////////////////////////////////////////////////////////////////////
Interference_Graph::Interference_Graph(){
        connect_registers();
}

Interference_Graph::Interference_Graph(compiler_ptr<Function> f){
        connect_registers();

        f->populate_liveness_sets();
        *this = f->make_interference_graph();
}

Interference_Graph::Interference_Graph(Function* f){
        connect_registers();

        f->populate_liveness_sets();
        *this = f->make_interference_graph();
}



void Interference_Graph::add_edge(std::string id_a, std::string id_b){
        nodes_set.insert({id_a, Lang_Constants::regs_set.count(id_a) ?
                                std::make_shared<IG_Node>(id_a, string_to_GPRcolor(id_a)) :
                                std::make_shared<IG_Node>(id_a)});
        nodes_set.insert({id_b, Lang_Constants::regs_set.count(id_b) ?
                                std::make_shared<IG_Node>(id_b, string_to_GPRcolor(id_b)) :
                                std::make_shared<IG_Node>(id_b)});

        adjacency_set[id_a].insert(nodes_set[id_b]);
        adjacency_set[id_b].insert(nodes_set[id_a]);
}

void Interference_Graph::connect_registers(){
        for(auto reg : Lang_Constants::regs_vector){
                for(auto reg_prime : Lang_Constants::regs_vector){
                        add_edge(reg, reg_prime);
                }
        }
}

void Interference_Graph::
draw_intra_set_edges(const io_set_t &st){
        for(auto id : st){
                for(auto id_prime : st){
                        if(id != id_prime && !Lang_Constants::regs_set.count(id))
                                add_edge(id, id_prime);
                }
        }
}

using node_stack_t = std::vector<compiler_ptr<IG_Node>>;
node_stack_t
Interference_Graph::attempt_coloring(){

        // setting up stack for graph construction

        node_stack_t nodes_to_color_stack;

        for(auto node_ptr_pair : nodes_set){
                auto node_ptr = node_ptr_pair.second;
                assert(!node_ptr->get_color() ||
                       Lang_Constants::regs_set.count(node_ptr->get_name()));

                nodes_to_color_stack.push_back(node_ptr);
        }

        std::sort(nodes_to_color_stack.begin(),
                  nodes_to_color_stack.end(),
                  [this](const compiler_ptr<IG_Node>& a,
                         const compiler_ptr<IG_Node>& b){
                          return adjacency_set.at(a->get_name()).size() >
                                  adjacency_set.at(b->get_name()).size();
                  });

        node_stack_t spill_nodes;

        // Try to color graph
        // XXX danger will robinson. Mutation...
        for(auto node_ptr : nodes_to_color_stack){
                bool colored{false};
                for(auto color : Enum<GPR_Color>()){
                        if(!get_neighbor_colors(
                                   node_ptr->get_name()).count(color)){
                                node_ptr->set_color(color);
                                break;
                        }
                }

                if(!node_ptr->get_color()){
                        spill_nodes.push_back(node_ptr);
                }
        }

        return  spill_nodes;
}


std::set<GPR_Color> Interference_Graph::get_neighbor_colors(std::string id){
        std::set<GPR_Color> neighbor_colors;

        for(auto node_ptr : adjacency_set[id]){
                if(node_ptr->get_color()){
                        neighbor_colors.insert(*node_ptr->get_color());
                }
        }
        return neighbor_colors;
}

std::unordered_map<std::string, std::string> Interference_Graph::get_reg_map(){
        std::unordered_map<std::string, std::string> reg_map;
        for(auto node : nodes_set){
                auto node_ptr = node.second;
                reg_map[node.first] =
// this will throw if the node is not colored
                        GPR_Color_to_string(*node_ptr->get_color());
        }

        return reg_map;
}

std::ostream& L2::operator<<(std::ostream& out, const std::shared_ptr<IG_Node>& ptr){
        out << *ptr;
        return out;
}

std::ostream& L2::operator<<(std::ostream& out, const Interference_Graph& ig){
        out << ig.adjacency_set;
        return out;
}

///////////////////////////////////////////////////////////////////////////////
//                                  ig node_ptr                                  //
///////////////////////////////////////////////////////////////////////////////
IG_Node::IG_Node() = default;

IG_Node::IG_Node(std::string L2_ID) :
        L2_ID(L2_ID){}

IG_Node::IG_Node(std::string L2_ID, GPR_Color color) :
        L2_ID(L2_ID),
        color(color){}

boost::optional<GPR_Color> IG_Node::get_color() const{
        return color;
}

void IG_Node::set_color(GPR_Color c){
        color = c;
}

std::string IG_Node::get_name() const{
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



std::ostream& L2::operator<<(std::ostream& out, GPR_Color c){
        out << GPR_Color_to_string(c);
        return out;
}

std::ostream& L2::operator<<(std::ostream& out, IG_Node id){
        out << "(IG_Node: ";
        out << id.L2_ID << " ";
        if(id.color){
                out << GPR_Color_to_string(*id.color);
        } else{
                out << "nullopt";
        }
        out << ")";
        return out;
}


bool L2::operator==(const compiler_ptr<IG_Node>& node1,
                    const compiler_ptr<IG_Node>& node2){
        return node1->L2_ID == node2->L2_ID && node1->color == node2->color;
}
