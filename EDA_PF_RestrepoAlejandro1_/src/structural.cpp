#include "graph.hpp" // Importar la clase para representar el grafo
#include <vector> // Para usar vectores
#include <queue> // Para usar colas en BFS
#include <iostream> // Para entrada/salida
#include <fstream> // Para manejo de archivos

struct GraphStats { // Estructura para estadísticas del grafo
    int num_nodes;
    int num_edges;
    int main_component_size;
    double avg_degree;
    int max_degree_node;
    int max_degree;
    int num_components;
    int diameter_estimate;
};

// DFS iterativo para contar componentes conexas
void dfs(int start_node, const Graph& g, std::vector<bool>& visited, int& component_size) {
    std::vector<int> stack;
    stack.push_back(start_node);
    
    const auto& adj = g.get_adjacency_list(); // Obtener la lista de adyacencia del grafo
    
    while (!stack.empty()) { // Mientras haya nodos en la pila
        int node = stack.back();
        stack.pop_back();
        
        if (node < 0 || node >= (int)visited.size() || visited[node]) { // Verificar que el nodo sea válido y no haya sido visitado
            continue;
        }
        
        visited[node] = true;
        component_size++;
        
        if (node < (int)adj.size()) {
            for (const auto& edge : adj[node]) {
                if (edge.to >= 0 && edge.to < (int)visited.size() && !visited[edge.to]) {
                    stack.push_back(edge.to);
                }
            }
        }
    }
}

// BFS para estimar diámetro
int bfs_max_distance(int start, const Graph& g) { // BFS para encontrar la distancia máxima desde un nodo dado
    int n = g.get_num_nodes();
    std::vector<int> dist(n, -1);
    std::queue<int> q;
    
    q.push(start);
    dist[start] = 0;
    int max_dist = 0;
    
    const auto& adj = g.get_adjacency_list(); // Obtener la lista de adyacencia del grafo
    
    while (!q.empty()) { // Mientras haya nodos en la cola
        int u = q.front();
        q.pop();
        
        for (const auto& edge : adj[u]) { // Para cada arista adyacente a u
            int v = edge.to;
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                max_dist = std::max(max_dist, dist[v]);
                q.push(v);
            }
        }
    }
    
    return max_dist;
}

GraphStats analyze_graph(const Graph& g) { // Función para analizar la estructura del grafo y calcular estadísticas
    GraphStats stats;
    stats.num_nodes = g.get_num_nodes();
    stats.num_edges = g.get_num_edges();
    
    // Calcular grado promedio (dividir por 2 porque contamos bidireccional)
    long long total_degree = 0;
    stats.max_degree = 0;
    stats.max_degree_node = -1;
    
    for (int i = 0; i < stats.num_nodes; i++) {
        int degree = g.get_degree(i);
        total_degree += degree;
        if (degree > stats.max_degree) {
            stats.max_degree = degree;
            stats.max_degree_node = g.get_original_id(i);
        }
    }
    
    // El grado promedio es suma_grados / num_nodos
    // Pero como cada arista suma 2 al grado total, el grado promedio es:
    // total_degree / num_nodos = 2 * num_aristas_no_dirigidas / num_nodos
    stats.avg_degree = (stats.num_nodes > 0) ? (double)total_degree / stats.num_nodes : 0;
    
    // Contar componentes conexas
    std::vector<bool> visited(stats.num_nodes, false);
    stats.num_components = 0;
    int main_component_size = 0;
    
    for (int i = 0; i < stats.num_nodes; i++) {
        if (!visited[i]) {
            int comp_size = 0;
            dfs(i, g, visited, comp_size);
            stats.num_components++;
            if (comp_size > main_component_size) {
                main_component_size = comp_size;
            }
        }
    }
    
    stats.main_component_size = main_component_size;
    
    // Estimar diámetro desde nodo de máximo grado
    int diameter_node = g.get_internal_id(stats.max_degree_node);
    stats.diameter_estimate = bfs_max_distance(diameter_node, g);
    
    return stats;
}

void save_structural_analysis(const GraphStats& stats, const std::string& filename) { // Función para guardar el análisis estructural en un archivo de texto
    std::ofstream file(filename);
    
    file << "=== ANALISIS ESTRUCTURAL DEL GRAFO ===" << std::endl;
    file << std::endl;
    file << "Numero de nodos: " << stats.num_nodes << std::endl;
    file << "Numero de aristas dirigidas (almacenadas): " << stats.num_edges << std::endl;
    file << "Numero de aristas no dirigidas: " << (stats.num_edges / 2) << std::endl;
    file << "Nodos en la componente conexa principal: " << stats.main_component_size << std::endl;
    file << "Grado promedio: " << stats.avg_degree << std::endl;
    file << "Nodo de mayor grado: " << stats.max_degree_node << " (grado: " << stats.max_degree << ")" << std::endl;
    file << "Numero de componentes conexas: " << stats.num_components << std::endl;
    file << "Diametro estimado (desde nodo de mayor grado): " << stats.diameter_estimate << std::endl;
    file << std::endl;
    
    file << "=== COMPARACION CON VALORES SNAP ===" << std::endl;
    file << "Nodos totales - SNAP: 1088092, Obtenido: " << stats.num_nodes << std::endl;
    file << "Aristas no dirigidas - SNAP: 1541898, Obtenido: " << (stats.num_edges / 2) << std::endl;
    file << "Nodos WCC principal - SNAP: 1087562, Obtenido: " << stats.main_component_size << std::endl;
    file << "Grado promedio - SNAP: 2.83, Obtenido: " << stats.avg_degree << std::endl;
    file << "Diametro - SNAP: 782, Estimado: " << stats.diameter_estimate << std::endl;
    
    file.close();
}
