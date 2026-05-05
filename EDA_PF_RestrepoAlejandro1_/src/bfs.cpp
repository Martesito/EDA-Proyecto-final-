#include "graph.hpp" // Importar la clase para representar el grafo
#include <queue>     // Para usar colas en BFS
#include <vector>    // Para usar vectores
#include <climits>   // Para límites de enteros

struct BFSResult {  // Estructura para resultados de BFS
    std::vector<int> distances; // Distancia en número de saltos desde el nodo inicial
    std::vector<int> parent;    // Nodo padre en el camino más corto (en términos de saltos)
    int nodes_explored;         // Número de nodos explorados durante la búsqueda
    int max_distance;      // Distancia máxima encontrada (en términos de saltos)
};

BFSResult bfs(const Graph& g, int start_node) { // BFS clásico para contar saltos
    int n = g.get_num_nodes(); // Obtener el número de nodos en el grafo
    BFSResult result; // Inicializar el resultado
    // Inicializar distancias a infinito, padres a -1, y contadores a 0
    
    result.distances.assign(n, INT_MAX); 
    result.parent.assign(n, -1);
    result.nodes_explored = 0;
    result.max_distance = 0;
    
    // Manejar nodo no válido
    if (start_node < 0 || start_node >= n) {
        return result;
    }
    // Usar una cola para BFS
    std::queue<int> q;
    q.push(start_node);
    result.distances[start_node] = 0;
    
    const auto& adj = g.get_adjacency_list();
    // Realizar BFS
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.nodes_explored++;
        
        for (const auto& edge : adj[u]) { // Para BFS clásico, solo nos importa el nodo destino, no el peso
            int v = edge.to;
            if (result.distances[v] == INT_MAX) {
                result.distances[v] = result.distances[u] + 1;
                result.parent[v] = u;
                result.max_distance = std::max(result.max_distance, result.distances[v]);
                q.push(v);
            }
        }
    }
    
    return result;
}

BFSResult bfs_ponderado(const Graph& g, int start_node) { // BFS modificado para considerar pesos como saltos
    int n = g.get_num_nodes();
    BFSResult result;
    result.distances.assign(n, INT_MAX);
    result.parent.assign(n, -1);
    result.nodes_explored = 0;
    result.max_distance = 0;
    
    if (start_node < 0 || start_node >= n) { // Manejar nodo no válido
        return result;
    }
    
    std::queue<int> q; // Cola para BFS
    q.push(start_node);
    result.distances[start_node] = 0;
    
    const auto& adj = g.get_adjacency_list();
    
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        result.nodes_explored++;
        
        for (const auto& edge : adj[u]) { // Para BFS ponderado, consideramos el peso como el número de saltos
            int v = edge.to;
            if (result.distances[v] == INT_MAX) {
                result.distances[v] = result.distances[u] + edge.weight;
                result.parent[v] = u;
                result.max_distance = std::max(result.max_distance, result.distances[v]);
                q.push(v);
            }
        }
    }
    
    return result; // Retornamos el resultado con distancias ponderadas
}
