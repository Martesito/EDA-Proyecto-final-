#include "graph.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <set>

Graph load_graph(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("No se pudo abrir el archivo: " + filename);
    }

    Graph g;
    g.reserve_nodes(1100000);

    std::srand(42);

    std::set<std::pair<int,int>> seen_edges;
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::istringstream iss(line);
        int u_orig, v_orig;
        if (!(iss >> u_orig >> v_orig)) continue;
        if (u_orig == v_orig) continue;

      
