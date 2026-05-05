#include "graph.hpp" // Importar la clase para representar el grafo
#include <set> // Para usar conjuntos
#include <map> // Para usar mapas
#include <vector> // Para usar vectores
#include <algorithm> // Para usar algoritmos de ordenamiento
#include <fstream> // Para manejo de archivos
#include <iostream> // Para entrada/salida

struct Subgraph { // Estructura para representar un subgrafo inducido
    std::vector<int> nodes;
    std::vector<std::pair<int, std::pair<int, int>>> edges; // (weight, (u, v)) 
};

struct MSTResult { // Estructura para resultados del MST
    long long total_weight;
    int num_edges;
};

// Estructura Union-Find para Kruskal
class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    
public:
    UnionFind(int n) : parent(n), rank(n, 0) { // Constructor para inicializar Union-Find
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }
    
    int find(int x) { // Encontrar el representante del conjunto al que pertenece x
        if (parent[x] != x) {
            parent[x] = find(parent[x]);
        }
        return parent[x];
    }
    
    bool unite(int x, int y) { // Unir los conjuntos a los que pertenecen x e y, retornando true si se unieron y false si ya estaban en el mismo conjunto
        int px = find(x);
        int py = find(y);
        
        if (px == py) return false; // Ya están en el mismo conjunto
        
        if (rank[px] < rank[py]) { // Unir el conjunto de px al de py
            parent[px] = py;
        } else if (rank[px] > rank[py]) {
            parent[py] = px;
        } else {
            parent[py] = px;
            rank[px]++;
        }
        return true;
    }
};

// Extraer subgrafo inducido por un conjunto de nodos
Subgraph extract_subgraph(const std::vector<int>& nodes, const Graph& g) { // Función para extraer un subgrafo inducido por un conjunto de nodos
    Subgraph subg;
    subg.nodes = nodes;
    
    std::set<int> node_set(nodes.begin(), nodes.end());
    const auto& adj = g.get_adjacency_list();
    
    std::set<std::pair<int, int>> edges_set;
    
    for (int u_internal : nodes) { // Para cada nodo interno en el conjunto de nodos del subgrafo
        for (const auto& edge : adj[u_internal]) {
            int v_internal = edge.to;
            if (node_set.count(v_internal)) { // Si el nodo adyacente también está en el conjunto, agregamos la arista al subgrafo
                int u = u_internal;
                int v = v_internal;
                if (u > v) std::swap(u, v);
                edges_set.insert({u, v});
            }
        }
    }
    
    // Convertir a vector de pares ordenados con pesos
    for (const auto& [u, v] : edges_set) {
        const auto& adj_u = adj[u];
        for (const auto& edge : adj_u) {
            if (edge.to == v) {
                subg.edges.push_back({edge.weight, {u, v}});
                break;
            }
        }
    }
    
    return subg;
}

// Algoritmo de Kruskal para MST
MSTResult kruskal_mst(const Subgraph& subg) {
    MSTResult result{0, 0};
    
    // Crear mapeo de índices para Union-Find
    std::set<int> unique_nodes(subg.nodes.begin(), subg.nodes.end());
    std::map<int, int> node_to_idx;
    int idx = 0;
    for (int node : unique_nodes) {
        node_to_idx[node] = idx++;
    }
    
    UnionFind uf(unique_nodes.size());
    
    // Copiar y ordenar aristas por peso
    auto sorted_edges = subg.edges;
    std::sort(sorted_edges.begin(), sorted_edges.end());
    
    for (const auto& [weight, edge] : sorted_edges) {
        int u = node_to_idx[edge.first];
        int v = node_to_idx[edge.second];
        
        if (uf.unite(u, v)) {
            result.total_weight += weight;
            result.num_edges++;
        }
    }
    
    return result;
}

// Verificación de DAG usando DFS con colores (actualizado)
bool check_dag(const Subgraph& subg, const Graph& g) {
    std::map<int, int> color;  // 0=blanco, 1=gris, 2=negro
    
    for (int node : subg.nodes) {
        color[node] = 0;
    }
    
    const auto& adj = g.get_adjacency_list();
    std::set<int> node_set(subg.nodes.begin(), subg.nodes.end());
    
    // Simple cycle detection: si en el subgrafo hay cualquier ciclo
    // lo detectamos verificando si las aristas van hacia nodos visitados en el mismo camino
    for (int start_node : subg.nodes) {
        if (color[start_node] == 0) {
            std::vector<int> stack;
            stack.push_back(start_node);
            color[start_node] = 1;
            
            while (!stack.empty()) {
                int u = stack.back();
                bool found_unvisited = false;
                
                if (u < (int)adj.size()) {
                    for (const auto& edge : adj[u]) {
                        int v = edge.to;
                        if (node_set.count(v)) {
                            if (color[v] == 1) {
                                // Ciclo detectado
                                return false;
                            } else if (color[v] == 0) {
                                color[v] = 1;
                                stack.push_back(v);
                                found_unvisited = true;
                                break;
                            }
                        }
                    }
                }
                
                if (!found_unvisited) {
                    color[u] = 2;
                    stack.pop_back();
                }
            }
        }
    }
    
    return true;
}

// Guardar subgrafo en formato lista de adyacencia
void save_subgraph(const Subgraph& subg, const Graph& g, const std::string& filename) {
    std::ofstream file(filename);
    
    file << "# Subgrafo inducido por caminos Q01 y Q06" << std::endl;
    file << "# Formato: FromNodeId\\tToNodeId" << std::endl;
    
    std::set<std::pair<int, int>> edges_written;
    
    for (int u_internal : subg.nodes) { // Para cada nodo interno en el conjunto de nodos del subgrafo
        const auto& adj = g.get_adjacency_list();
        for (const auto& edge : adj[u_internal]) {
            int v_internal = edge.to;
            if (std::find(subg.nodes.begin(), subg.nodes.end(), v_internal) != subg.nodes.end()) {
                int u_orig = g.get_original_id(u_internal);
                int v_orig = g.get_original_id(v_internal);
                
                if (u_orig < v_orig) {
                    auto key = std::make_pair(u_orig, v_orig);
                    if (edges_written.find(key) == edges_written.end()) {
                        file << u_orig << "\t" << v_orig << std::endl;
                        edges_written.insert(key);
                    }
                }
            }
        }
    }
    
    file.close();
}

// Guardar análisis del subgrafo
void save_subgraph_analysis(const Subgraph& subg, const MSTResult& mst, bool is_dag_graph, 
                            const std::string& filename) {
    std::ofstream file(filename);
    // Guardar análisis del subgrafo inducido en un archivo de texto
    file << "=== ANALISIS DEL SUBGRAFO INDUCIDO ===" << std::endl;
    file << std::endl;
    file << "Numero de nodos: " << subg.nodes.size() << std::endl;
    file << "Numero de aristas: " << subg.edges.size() << std::endl;
    file << std::endl;
    
    file << "=== ARBOL DE EXPANSION MINIMA (MST) ===" << std::endl;
    file << "Numero de aristas en MST: " << mst.num_edges << std::endl;
    file << "Peso total del MST: " << mst.total_weight << std::endl;
    file << std::endl;
    
    file << "=== ANALISIS DAG ===" << std::endl;
    file << "Es DAG (grafo aciclico dirigido): " << (is_dag_graph ? "SI" : "NO") << std::endl;
    file << std::endl;
    
    file << "Notas:" << std::endl;
    file << "- El subgrafo inducido contiene los nodos que aparecen en los caminos Q01 y Q06." << std::endl;
    file << "- El MST fue calculado usando el algoritmo de Kruskal." << std::endl;
    file << "- La verificacion de DAG se realizo con DFS." << std::endl;
    
    file.close();
}
