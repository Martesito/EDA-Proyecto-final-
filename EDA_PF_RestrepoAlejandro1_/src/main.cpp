#include "graph.hpp" // Clase para representar el grafo
#include <iostream>  // Para entrada/salida
#include <fstream>   // Para manejo de archivos
#include <vector>    // Para usar vectores
#include <chrono>    // Para medir tiempos
#include <climits>   // Para límites de enteros
#include <cmath>     // Para funciones matemáticas
#include <iomanip>   // Para formatear salida
#include <algorithm> // Para algoritmos de ordenamiento
#include <string>    // Para manejo de strings

// Forward declarations de funciones en otros módulos
Graph load_graph(const std::string& filename);

struct BFSResult { // Estructura para resultados de BFS
    std::vector<int> distances; // Distancia en número de saltos desde el nodo inicial
    std::vector<int> parent; // Nodo padre en el camino más corto (en términos de saltos)
    int nodes_explored; // Número de nodos explorados durante la búsqueda
    int max_distance; // Distancia máxima encontrada (en términos de saltos)
};
BFSResult bfs(const Graph& g, int start_node); // BFS clásico para contar saltos

struct DijkstraResult { // Estructura para resultados de Dijkstra
    std::vector<long long> distances;
    std::vector<int> parent;
    int nodes_explored;
};
DijkstraResult dijkstra(const Graph& g, int start_node); // Implementación de Dijkstra usando una cola de prioridad
std::vector<int> reconstruct_path(const DijkstraResult& result, int start, int end, const Graph& g); // Función para reconstruir el camino más corto a partir de los resultados de Dijkstra

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
GraphStats analyze_graph(const Graph& g); // Función para analizar la estructura del grafo y calcular estadísticas
void save_structural_analysis(const GraphStats& stats, const std::string& filename); // Función para guardar el análisis estructural en un archivo de texto

struct Subgraph { // Estructura para representar un subgrafo inducido
    std::vector<int> nodes; // Nodos del subgrafo (índices internos)
    std::vector<std::pair<int, std::pair<int, int>>> edges; // Aristas del subgrafo (weight, (u, v))
};

struct MSTResult { // Estructura para resultados del MST
    long long total_weight;
    int num_edges;
};
Subgraph extract_subgraph(const std::vector<int>& nodes, const Graph& g); // Función para extraer un subgrafo inducido por un conjunto de nodos
MSTResult kruskal_mst(const Subgraph& subg); // Función para calcular el árbol de expansión mínima (MST) de un subgrafo usando el algoritmo de Kruskal
bool check_dag(const Subgraph& subg, const Graph& g); // Función para verificar si el subgrafo inducido es un DAG (grafo acíclico dirigido)
void save_subgraph(const Subgraph& subg, const Graph& g, const std::string& filename); // Función para guardar el subgrafo inducido en un archivo de texto
void save_subgraph_analysis(const Subgraph& subg, const MSTResult& mst, bool is_dag_graph, // Función para guardar el análisis del subgrafo inducido en un archivo de texto
                            const std::string& filename); 

// Función para guardar tiempos
void save_timing(const std::string& filename, double time_a, double time_b, double time_c) { // Guardar tiempos de ejecución en un archivo CSV
    std::ofstream file(filename);
    
    file << "modulo,tiempo_ms" << std::endl; // Escribir encabezado del CSV
    if (time_a > 0) {
        file << "A (Analisis Estructural)," << std::fixed << std::setprecision(2) << time_a << std::endl; // Escribir tiempo del módulo A si es mayor a 0
    }
    if (time_b > 0) {
        file << "B (Consultas P2P)," << std::fixed << std::setprecision(2) << time_b << std::endl; // Escribir tiempo del módulo B si es mayor a 0
    }
    if (time_c > 0) {
        file << "C (Subgrafo MST DAG)," << std::fixed << std::setprecision(2) << time_c << std::endl; // Escribir tiempo del módulo C si es mayor a 0
    }
    if (time_a > 0 && time_b > 0 && time_c > 0) {
        file << "TOTAL," << std::fixed << std::setprecision(2) << (time_a + time_b + time_c) << std::endl; // Escribir tiempo total si los tiempos de los tres módulos son mayores a 0
    }
    
    file.close();
}

// Modulo A: Análisis estructural
double module_a(const std::string& dataset_file) { // Función para ejecutar el módulo A de análisis estructural
    auto start_module = std::chrono::high_resolution_clock::now(); // Iniciar el cronómetro para medir el tiempo de ejecución del módulo A
    
    std::cout << "[MODULO A] Cargando grafo..." << std::endl; // Imprimir mensaje indicando que se está cargando el grafo
    Graph g = load_graph(dataset_file); // Cargar el grafo desde el archivo de entrada
    
    std::cout << "[MODULO A] Analizando estructura..." << std::endl; // Imprimir mensaje indicando que se está analizando la estructura del grafo
    GraphStats stats = analyze_graph(g); // Analizar la estructura del grafo y calcular estadísticas relevantes
    
    std::cout << "[MODULO A] Guardando resultados..." << std::endl; // Imprimir mensaje indicando que se están guardando los resultados del análisis estructural
    save_structural_analysis(stats, "results/analisis_estructural.txt"); // Guardar el análisis estructural en un archivo de texto
    
    auto end_module = std::chrono::high_resolution_clock::now(); // Detener el cronómetro para medir el tiempo de ejecución del módulo A
    double time_ms = std::chrono::duration<double, std::milli>(end_module - start_module).count(); // Calcular el tiempo de ejecución en milisegundos
    
    std::cout << "[MODULO A] Completado en " << std::fixed << std::setprecision(2) << time_ms << " ms" << std::endl; // Imprimir mensaje indicando que el módulo A se ha completado y mostrar el tiempo de ejecución formateado a 2 decimales
    std::cout << "  - Nodos: " << stats.num_nodes << std::endl; // Imprimir el número de nodos en el grafo
    std::cout << "  - Aristas: " << stats.num_edges << std::endl; // Imprimir el número de aristas en el grafo
    std::cout << "  - Componente principal: " << stats.main_component_size << std::endl; // Imprimir el tamaño de la componente conexa principal
    
    return time_ms; // Retornar el tiempo de ejecución del módulo A en milisegundos
}

// Estructura para una consulta P2P
struct Query { // Estructura para representar una consulta P2P
    int id;
    int source;
    int destination;
};

// Modulo B: Consultas P2P con Dijkstra y BFS
double module_b(const std::string& dataset_file) { // Función para ejecutar el módulo B de consultas P2P usando Dijkstra y BFS
    auto start_module = std::chrono::high_resolution_clock::now(); // Iniciar el cronómetro para medir el tiempo de ejecución del módulo B
    
    std::cout << "[MODULO B] Cargando grafo..." << std::endl; // Imprimir mensaje indicando que se está cargando el grafo
    Graph g = load_graph(dataset_file);
    
    // Crear conjunto de consultas
    std::vector<Query> queries = {
        {1, 1, 500000},
        {2, 100, 1000000},
        {3, 50000, 750000},
        {4, 200000, 800000},
        {5, 300000, 100000},
        {6, 1, 1087562},
        {7, 500000, 1},
        {8, 250000, 600000},
        {9, 10000, 900000},
        {10, 400000, 150000}
    };
    
    std::ofstream csv_file("results/consultas_p2p.csv"); // Abrir archivo CSV para guardar los resultados de las consultas
    csv_file << "consulta,origen,destino,dist_dijkstra,saltos_bfs,nodos_dijkstra,nodos_bfs,t_dijkstra_ms,t_bfs_ms" << std::endl; // Escribir encabezado del CSV
    
    std::cout << "[MODULO B] Ejecutando " << queries.size() << " consultas..." << std::endl; // Imprimir mensaje indicando que se están ejecutando las consultas P2P
    
    for (const auto& query : queries) { // Para cada consulta en el conjunto de consultas
        int src_internal = g.get_internal_id(query.source);
        int dst_internal = g.get_internal_id(query.destination);
        
        if (src_internal == -1 || dst_internal == -1) { // Si el nodo de origen o destino no existe en el grafo, manejar el caso de nodo no encontrado
            std::cout << "  Q" << std::setfill('0') << std::setw(2) << query.id // Imprimir mensaje indicando que la consulta no se puede ejecutar porque el nodo de origen o destino no existe
                      << ": Nodo no existe" << std::endl;
            csv_file << "Q" << std::setfill('0') << std::setw(2) << query.id << "," // Escribir en el CSV que la consulta no se puede ejecutar porque el nodo de origen o destino no existe
                     << query.source << "," << query.destination << ",INF,INF,0,0,0,0" << std::endl;
            continue;
        }
        
        // Ejecutar Dijkstra
        auto start_dij = std::chrono::high_resolution_clock::now();
        DijkstraResult dijkstra_result = dijkstra(g, src_internal);
        auto end_dij = std::chrono::high_resolution_clock::now();
        auto time_dijkstra = std::chrono::duration<double, std::milli>(end_dij - start_dij).count();
        
        // Ejecutar BFS
        auto start_bfs = std::chrono::high_resolution_clock::now();
        BFSResult bfs_result = bfs(g, src_internal);
        auto end_bfs = std::chrono::high_resolution_clock::now();
        auto time_bfs = std::chrono::duration<double, std::milli>(end_bfs - start_bfs).count();
        
        long long dist_dijkstra = dijkstra_result.distances[dst_internal];
        int saltos_bfs = bfs_result.distances[dst_internal];
        
        csv_file << "Q" << std::setfill('0') << std::setw(2) << query.id << "," // Escribir los resultados de la consulta en el CSV
                 << query.source << "," << query.destination << ",";
        
        if (dist_dijkstra == LLONG_MAX) { // Si no hay camino entre el nodo de origen y destino, escribir "INF" en el CSV para indicar que la distancia es infinita
            csv_file << "INF,INF," << dijkstra_result.nodes_explored << "," 
                     << bfs_result.nodes_explored << ",";
        } else {
            csv_file << dist_dijkstra << "," << saltos_bfs << "," 
                     << dijkstra_result.nodes_explored << "," 
                     << bfs_result.nodes_explored << ",";
        }
        
        csv_file << std::fixed << std::setprecision(2) 
                 << time_dijkstra << "," << time_bfs << std::endl;
        
        std::cout << "  Q" << std::setfill('0') << std::setw(2) << query.id << ": " // Imprimir los resultados de la consulta en la consola
                  << (dist_dijkstra == LLONG_MAX ? "INF" : std::to_string(dist_dijkstra))
                  << " (Dijkstra) / " << saltos_bfs << " (BFS)" << std::endl;
    }
    
    csv_file.close(); // Cerrar el archivo CSV después de escribir los resultados de las consultas
    
    auto end_module = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end_module - start_module).count();
    
    std::cout << "[MODULO B] Completado en " << std::fixed << std::setprecision(2) << time_ms << " ms" << std::endl;
    
    return time_ms;
}

// Modulo C: Subgrafo, MST y DAG
double module_c(const std::string& dataset_file) {
    auto start_module = std::chrono::high_resolution_clock::now();
    
    std::cout << "[MODULO C] Cargando grafo..." << std::endl;
    Graph g = load_graph(dataset_file);
    
    // Ejecutar Dijkstra desde Q01 (1 -> 500000) y Q06 (1 -> 1087562)
    int src1_internal = g.get_internal_id(1);
    int dst1_internal = g.get_internal_id(500000);
    int dst2_internal = g.get_internal_id(1087562);
    
    std::vector<int> all_path_nodes;
    
    if (src1_internal != -1 && dst1_internal != -1) { // Verificar que los nodos de origen y destino para Q01 existan antes de ejecutar Dijkstra
        std::cout << "[MODULO C] Reconstruyendo camino Q01..." << std::endl;
        DijkstraResult result1 = dijkstra(g, src1_internal);
        auto path1 = reconstruct_path(result1, src1_internal, dst1_internal, g);
        all_path_nodes.insert(all_path_nodes.end(), path1.begin(), path1.end());
        std::cout << "  Camino Q01: " << path1.size() << " nodos" << std::endl;
    }
    
    if (src1_internal != -1 && dst2_internal != -1) {
        std::cout << "[MODULO C] Reconstruyendo camino Q06..." << std::endl;
        DijkstraResult result2 = dijkstra(g, src1_internal);
        auto path2 = reconstruct_path(result2, src1_internal, dst2_internal, g);
        all_path_nodes.insert(all_path_nodes.end(), path2.begin(), path2.end());
        std::cout << "  Camino Q06: " << path2.size() << " nodos" << std::endl;
    }
    
    // Eliminar duplicados
    std::sort(all_path_nodes.begin(), all_path_nodes.end());
    all_path_nodes.erase(std::unique(all_path_nodes.begin(), all_path_nodes.end()), all_path_nodes.end());
    
    // Convertir a índices internos
    std::vector<int> internal_nodes;
    for (int node : all_path_nodes) {
        int internal = g.get_internal_id(node);
        if (internal != -1) {
            internal_nodes.push_back(internal);
        }
    }
    
    std::cout << "[MODULO C] Extrayendo subgrafo con " << internal_nodes.size() << " nodos..." << std::endl; // Extraer el subgrafo inducido por los nodos encontrados en los caminos Q01 y Q06
    Subgraph subg = extract_subgraph(internal_nodes, g);
    
    std::cout << "[MODULO C] Calculando MST (Kruskal)..." << std::endl;
    MSTResult mst = kruskal_mst(subg);
    
    std::cout << "[MODULO C] Verificando DAG..." << std::endl;
    bool is_dag_graph = check_dag(subg, g);
    
    std::cout << "[MODULO C] Guardando resultados..." << std::endl;
    save_subgraph(subg, g, "results/subgrafo_caminos.txt");
    save_subgraph_analysis(subg, mst, is_dag_graph, "results/analisis_subgrafo.txt");
    
    auto end_module = std::chrono::high_resolution_clock::now();
    double time_ms = std::chrono::duration<double, std::milli>(end_module - start_module).count();
    
    std::cout << "[MODULO C] Completado en " << std::fixed << std::setprecision(2) << time_ms << " ms" << std::endl; // Imprimir mensaje indicando que el módulo C se ha completado y mostrar el tiempo de ejecución formateado a 2 decimales
    std::cout << "  - Nodos subgrafo: " << subg.nodes.size() << std::endl;
    std::cout << "  - Aristas subgrafo: " << subg.edges.size() << std::endl;
    std::cout << "  - Peso MST: " << mst.total_weight << std::endl;
    std::cout << "  - Es DAG: " << (is_dag_graph ? "SI" : "NO") << std::endl;
    
    return time_ms;
}

int main(int argc, char* argv[]) { // Función principal del programa
    if (argc < 3) {
        std::cerr << "Uso: " << argv[0] << " <archivo_dataset> <modulo>" << std::endl;
        std::cerr << "Modulos:" << std::endl;
        std::cerr << "  A - Analisis estructural" << std::endl;
        std::cerr << "  B - Consultas P2P (Dijkstra vs BFS)" << std::endl;
        std::cerr << "  C - Subgrafo, MST y DAG" << std::endl;
        std::cerr << "  ALL - Ejecutar todos los modulos" << std::endl;
        return 1;
    }
    
    std::string dataset_file = argv[1];
    std::string module = argv[2];
    
    double time_a = 0, time_b = 0, time_c = 0;
    
    try {
        if (module == "A" || module == "a") { // Ejecutar módulo A de análisis estructural
            time_a = module_a(dataset_file);
            save_timing("results/tiempos.csv", time_a, 0, 0);
        } else if (module == "B" || module == "b") { // Ejecutar módulo B de consultas P2P usando Dijkstra y BFS
            time_b = module_b(dataset_file);
            save_timing("results/tiempos.csv", 0, time_b, 0);
        } else if (module == "C" || module == "c") { // Ejecutar módulo C de subgrafo, MST y DAG
            time_c = module_c(dataset_file);
            save_timing("results/tiempos.csv", 0, 0, time_c);
        } else if (module == "ALL" || module == "all") { // Ejecutar todos los módulos A, B y C
            time_a = module_a(dataset_file);
            time_b = module_b(dataset_file);
            time_c = module_c(dataset_file);
            save_timing("results/tiempos.csv", time_a, time_b, time_c); // Guardar los tiempos de ejecución de los tres módulos en un archivo CSV
        } else {
            std::cerr << "Modulo desconocido: " << module << std::endl; // Manejar el caso de módulo desconocido
            return 1;
        }
    } catch (const std::exception& e) { // Manejar cualquier excepción que ocurra durante la ejecución de los módulos y mostrar un mensaje de error
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
