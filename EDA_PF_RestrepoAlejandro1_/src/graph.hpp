#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_map>
#include <utility>



/**
 * Representa una arista con peso
 */
struct Edge {
    int to;           // Nodo destino
    int weight;       // Peso de la arista
};



/**
 * Representa un grafo no dirigido ponderado
 * Usa lista de adyacencia como estructura de datos interna
 */
class Graph {
private:
    std::vector<std::vector<Edge>> adj_list;    // Lista de adyacencia
    std::unordered_map<int, int> node_mapping;  // ID original -> índice interno
    std::vector<int> reverse_mapping;           // Índice interno -> ID original
    int num_nodes;
    int num_edges;

public:
    // Constructor
    Graph();

    void add_edge(int u, int v, int weight);

   
    const std::vector<std::vector<Edge>>& get_adjacency_list() const;

  
    int get_internal_id(int original_id) const;

   
    int get_original_id(int internal_id) const;

    // Getters
    int get_num_nodes() const;
    int get_num_edges() const;
};

#endif // GRAPH_HPP

