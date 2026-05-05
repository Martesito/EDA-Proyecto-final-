#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_map>
#include <utility>

// Estructura que representa una arista con peso
struct Edge {
    int to;
    int weight;
};

// Clase principal para representar el grafo
class Graph {
private:
    std::vector<std::vector<Edge>> adj_list;        // Lista de adyacencia ponderada
    std::unordered_map<int, int> node_mapping;      // Mapeo: ID original -> índice interno
    std::vector<int> reverse_mapping;               // Mapeo inverso: índice interno -> ID original
    int num_nodes;
    int num_edges;

public:
    Graph();
    
    // Agregar una arista al grafo (no dirigida)
    void add_edge(int u, int v, int weight);
    
    // Obtener lista de adyacencia
    const std::vector<std::vector<Edge>>& get_adjacency_list() const;
    
    // Obtener el índice interno de un nodo
    int get_internal_id(int original_id) const;
    
    // Obtener el ID original de un nodo
    int get_original_id(int internal_id) const;
    
    // Obtener número de nodos
    int get_num_nodes() const;
    
    // Obtener número de aristas
    int get_num_edges() const;
    
    // Verificar si un nodo existe (por ID original)
    bool node_exists(int node_id) const;
    
    // Obtener el grado de un nodo (por índice interno)
    int get_degree(int internal_id) const;
    
    // Mapear un nodo original a índice interno
    void map_node(int original_id, int internal_id);
};

#endif
