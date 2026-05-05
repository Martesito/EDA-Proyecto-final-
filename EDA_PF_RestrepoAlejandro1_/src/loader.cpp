#include "graph.hpp"
#include <iostream>
#include <fstream>
#include <string>



/**
 * Carga un grafo desde un archivo de texto
 * Formato: cada línea contiene "nodo1 nodo2 peso"
 * 
 * @param filename Ruta del archivo
 * @return Grafo cargado
 */
Graph load_graph(const std::string& filename) {
    Graph g;
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }

    

    file.close();
    return g;
}
