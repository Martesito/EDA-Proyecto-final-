#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <string>



using namespace std;


class Graph {
private:
    
    vector<vector<pair<int, int>>> adj_list;  
    int num_nodes;
    int num_edges;

public:
    Graph() : num_nodes(0), num_edges(0) {}

    void add_edge(int u, int v, int weight) {
       
    }

    int get_num_nodes() const { return num_nodes; }
    int get_num_edges() const { return num_edges; }
};



Graph load_graph(const string& filename) {
    Graph g;
    
    return g;
}


void print_graph_info(const Graph& g) {
    cout << "=== Información del Grafo ===" << endl;
    cout << "Nodos: " << g.get_num_nodes() << endl;
    cout << "Aristas: " << g.get_num_edges() << endl;
}



int main() {
    cout << "=== Análisis de Grafos - EDA Proyecto Final ===" << endl;
    cout << endl;

    try {
       
        string data_path = "../data/roadNet-PA.txt";
        cout << "Cargando grafo desde: " << data_path << endl;
        
        auto start_time = chrono::high_resolution_clock::now();
        Graph g = load_graph(data_path);
        auto end_time = chrono::high_resolution_clock::now();

        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time);
        cout << "Grafo cargado en " << duration.count() << " ms" << endl;
        cout << endl;

       
        print_graph_info(g);
        cout << endl;

        // TODO COMMIT 2: Integrar todos los algoritmos
        // - Ejecutar BFS desde nodos seleccionados
        // - Ejecutar Dijkstra desde nodos seleccionados
        // - Realizar análisis estructural
        // - Extraer y analizar subgrafos

    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}




