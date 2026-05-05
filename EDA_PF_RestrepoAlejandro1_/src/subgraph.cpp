#include "graph.hpp"
#include <vector>
#include <algorithm>



/**
 * Representa un subgrafo inducido
 */
struct Subgraph {
    std::vector<int> nodes;                              // Nodos del subgrafo
    std::vector<std::pair<int, std::pair<int, int>>> edges;  // (peso, (u, v))
};

/**
 * Resultado del Árbol de Expansión Mínima (MST)
 */
struct MSTResult {
    long long total_weight;         // Peso total del MST
    std::vector<std::pair<int, int>> edges;  // Aristas del MST
};



/**
 * Extrae un subgrafo inducido por un conjunto de nodos
 * 
 * @param g Grafo original
 * @param nodes Conjunto de nodos
 * @return Subgrafo inducido
 */
Subgraph extract_subgraph(const Graph& g, const std::vector<int>& nodes) {
    Subgraph subgraph;
    
   
    
    return subgraph;
}

/**
 * Calcula el Árbol de Expansión Mínima usando Kruskal o Prim
 * 
 * @param g Grafo ponderado
 * @return Resultado del MST
 */
MSTResult compute_mst(const Graph& g) {
    MSTResult result;
    
    
    
    return result;
}

/**
 * Encuentra componentes conexas del grafo
 * 
 * @param g Grafo
 * @return Vector de componentes (cada componente es un vector de nodos)
 */
std::vector<std::vector<int>> find_connected_components(const Graph& g) {
    std::vector<std::vector<int>> components;
    
    return components;
}
