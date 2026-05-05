#include "graph.hpp"
#include <vector>
#include <queue>
#include <climits>



struct DijkstraResult {
    std::vector<long long> distances;  
    std::vector<int> parent;           
    int nodes_explored;                
};



/**
 * Algoritmo de Dijkstra para encontrar caminos más cortos
 * Usa una cola de prioridad para optimizar la búsqueda
 * 
 * @param g Grafo ponderado
 * @param start_node Nodo inicial
 * @return Resultado del algoritmo Dijkstra
 * 
 * 
 */
DijkstraResult dijkstra(const Graph& g, int start_node) {
    DijkstraResult result;
    
    
    
    
  
    
    return result;
}


std::vector<int> reconstruct_path_dijkstra(const DijkstraResult& result, int start, int end) {
    std::vector<int> path;
    
    return path;
}
