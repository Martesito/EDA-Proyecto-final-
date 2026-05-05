#include "graph.hpp" // Importar la clase para representar el grafo
#include <queue>    // Para usar colas en BFS
#include <vector>   // Para usar vectores
#include <climits>  // Para límites de enteros
#include <utility>  // Para usar std::pair
#include <algorithm> // Para usar std::reverse

struct DijkstraResult { // Estructura para resultados de Dijkstra
    std::vector<long long> distances; // Distancia mínima desde el nodo inicial a cada nodo
    std::vector<int> parent;  // Nodo padre en el camino más corto
    int nodes_explored; // Número de nodos explorados durante la búsqueda
};

DijkstraResult dijkstra(const Graph& g, int start_node) { // Implementación de Dijkstra usando una cola de prioridad
    int n = g.get_num_nodes();
    DijkstraResult result;
    result.distances.assign(n, LLONG_MAX);
    result.parent.assign(n, -1);
    result.nodes_explored = 0;
    
    if (start_node < 0 || start_node >= n) { // Manejar nodo no válido
        return result;
    }
    
    result.distances[start_node] = 0;
    
    // Cola de prioridad para seleccionar el nodo con la distancia mínima
    std::priority_queue<std::pair<long long, int>, 
                       std::vector<std::pair<long long, int>>,
                       std::greater<std::pair<long long, int>>> pq;
    
    pq.push({0, start_node});
    
    const auto& adj = g.get_adjacency_list(); // Obtener la lista de adyacencia del grafo
    std::vector<bool> visited(n, false); // Vector para marcar nodos visitados
    
    while (!pq.empty()) { // Mientras haya nodos en la cola de prioridad
        auto [dist, u] = pq.top();
        pq.pop();
        
        if (visited[u]) {
            continue;
        }
        
        visited[u] = true;
        result.nodes_explored++;
        
        for (const auto& edge : adj[u]) { // Para cada arista adyacente a u
            int v = edge.to;
            long long new_dist = result.distances[u] + edge.weight;
            
            if (new_dist < result.distances[v]) { // Si encontramos un camino más corto a v
                result.distances[v] = new_dist;
                result.parent[v] = u;
                pq.push({new_dist, v});
            }
        }
    }
    
    return result; // Retornamos el resultado con distancias mínimas y nodos explorados
}

std::vector<int> reconstruct_path(const DijkstraResult& result, int start, int end, const Graph& g) { // Reconstruir el camino más corto desde start hasta end usando el vector parent
    std::vector<int> path;
    
    if (result.distances[end] == LLONG_MAX) { // Si no hay camino, retornamos un vector vacío
        return path; // Sin camino
    }
    
    int current = end;
    while (current != -1) { // Mientras no lleguemos al nodo inicial
        path.push_back(g.get_original_id(current));
        current = result.parent[current];
    }
    
    std::reverse(path.begin(), path.end()); // El camino se construyó al revés, así que lo invertimos
    return path;
}
