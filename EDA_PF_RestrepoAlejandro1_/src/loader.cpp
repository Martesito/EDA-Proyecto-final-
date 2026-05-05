#include "graph.hpp" // Importar la clase para representar el grafo
#include <fstream>   // Para manejo de archivos
#include <sstream>  // Para usar istringstream
#include <cstdlib>  // Para usar rand() y srand()
#include <iostream> // Para entrada/salida
#include <set> // Para usar conjuntos

Graph load_graph(const std::string& filename) { // Función para cargar el grafo desde un archivo
    Graph g;
    std::ifstream file(filename); // Abrir el archivo de entrada
    
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename); // Manejar error si el archivo no se puede abrir
    }
    
    // Inicializar semilla para pesos aleatorios reproducibles
    srand(42);
    
    std::string line;
    int internal_id = 0;
    std::set<int> seen_nodes;
    
    while (std::getline(file, line)) { // Leer el archivo línea por línea
        // Ignorar líneas de comentario
        if (line.empty() || line[0] == '#') { // Continuar con la siguiente iteración si la línea es vacía o es un comentario
            continue;
        }
        
        std::istringstream iss(line); // Usar istringstream para parsear la línea
        int u, v;
        
        if (!(iss >> u >> v)) { // Manejar error si la línea no tiene el formato esperado
            continue;
        }
        
        // Mapear nodos si no existen
        if (!g.node_exists(u)) { // Si el nodo u no existe, mapearlo a un índice interno
            g.map_node(u, internal_id++); // Mapear el nodo u al índice interno actual y luego incrementar el índice
        }
        if (!g.node_exists(v)) { // Si el nodo v no existe, mapearlo a un índice interno
            g.map_node(v, internal_id++); // Mapear el nodo v al índice interno actual y luego incrementar el índice
        }
        
        // Generar peso aleatorio uniforme [1, 10]
        int weight = (rand() % 10) + 1;
        
        // Obtener IDs internos
        int u_internal = g.get_internal_id(u);
        int v_internal = g.get_internal_id(v);
        
        // Agregar arista no dirigida
        g.add_edge(u_internal, v_internal, weight);
    }
    
    file.close(); // Cerrar el archivo después de leerlo
    return g; // Retornar el grafo cargado
}
