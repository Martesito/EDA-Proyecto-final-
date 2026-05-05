#include "graph.hpp"
#include <vector>
#include <queue>
#include <climits>


struct BFSResult {
    std::vector<int> distances;      // Distancia (en saltos) desde el nodo inicial
    std::vector<int> parent;         // Nodo padre en el árbol BFS
    int nodes_explored;              // Cantidad de nodos explorados
    int max_distance;                // Máxima distancia encontrada
};



/**
 * Búsqueda en amplitud (BFS)
 * Encuentra caminos más cortos en términos de número de saltos
 * 
 * @param g Grafo a explorar
 * @param start_node Nodo inicial
 * @return Resultado del BFS
 */
BFSResult bfs(const Graph& g, int start_node) {
    BFSResult result;
    
   
    
    return result;
}

/**
 * Reconstruye el camino desde start_node hasta end_node
 * usando los resultados de BFS
 */
std::vector<int> reconstruct_path_bfs(const BFSResult& result, int start, int end) {
    std::vector<int> path;
    
   
    return path;
}
