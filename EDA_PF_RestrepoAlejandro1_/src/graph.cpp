#include "graph.hpp"
#include <iostream>



Graph::Graph() : num_nodes(0), num_edges(0) {
    // Inicializar el grafo vacío
}

void Graph::add_edge(int u, int v, int weight) {
    
}

const std::vector<std::vector<Edge>>& Graph::get_adjacency_list() const {
    return adj_list;
}

int Graph::get_internal_id(int original_id) const {
  
    return -1;
}

int Graph::get_original_id(int internal_id) const {
  
    return -1;
}

int Graph::get_num_nodes() const {
    return num_nodes;
}

int Graph::get_num_edges() const {
    return num_edges;
}
