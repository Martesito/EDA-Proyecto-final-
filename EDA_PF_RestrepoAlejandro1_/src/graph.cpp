#include "graph.hpp" // Importar la clase para representar el grafo
#include <algorithm> // Para usar std::max

Graph::Graph() : num_nodes(0), num_edges(0) {} // Constructor para inicializar el grafo vacío

void Graph::add_edge(int u, int v, int weight) { // Agregar una arista no dirigida al grafo
    if (u >= (int)adj_list.size()) { // Asegurarnos de que la lista de adyacencia tenga suficiente espacio para los nodos u y v
        adj_list.resize(u + 1);
    }
    if (v >= (int)adj_list.size()) { // Redimensionar la lista de adyacencia si es necesario
        adj_list.resize(v + 1);
    }
    // Solo agregar la dirección u→v (el archivo ya tiene ambas)
    adj_list[u].push_back({v, weight});
    num_edges++;
    num_nodes = std::max(num_nodes, std::max(u, v) + 1); // Actualizar el número de nodos si es necesario
}

const std::vector<std::vector<Edge>>& Graph::get_adjacency_list() const { // Retornar la lista de adyacencia del grafo
    return adj_list;
}

int Graph::get_internal_id(int original_id) const { // Obtener el índice interno de un nodo dado su ID original
    auto it = node_mapping.find(original_id); // Buscar el ID original en el mapeo
    if (it != node_mapping.end()) {
        return it->second;
    }
    return -1;
}

int Graph::get_original_id(int internal_id) const { // Obtener el ID original de un nodo dado su índice interno
    if (internal_id >= 0 && internal_id < (int)reverse_mapping.size()) { // Verificar que el índice interno sea válido
        return reverse_mapping[internal_id];
    }
    return -1;
}

int Graph::get_num_nodes() const { // Retornar el número de nodos en el grafo
    return num_nodes;
}

int Graph::get_num_edges() const { // Retornar el número de aristas en el grafo
    return num_edges;
}

bool Graph::node_exists(int node_id) const { // Verificar si un nodo existe en el grafo dado su ID original
    return node_mapping.find(node_id) != node_mapping.end();
}

int Graph::get_degree(int internal_id) const { // Obtener el grado de un nodo dado su índice interno
    if (internal_id >= 0 && internal_id < (int)adj_list.size()) {
        return adj_list[internal_id].size();
    }
    return 0; // Si el nodo no existe, su grado es 0
}

void Graph::map_node(int original_id, int internal_id) { // Mapear un nodo original a un índice interno
    if (internal_id >= (int)adj_list.size()) {
        adj_list.resize(internal_id + 1);
        reverse_mapping.resize(internal_id + 1);
    }
    node_mapping[original_id] = internal_id; // Guardar el mapeo del ID original al índice interno
    reverse_mapping[internal_id] = original_id;
    if (internal_id + 1 > num_nodes) {
        num_nodes = internal_id + 1;
    }
}
