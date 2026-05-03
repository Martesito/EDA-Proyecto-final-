#include "GraphLoader.hpp"
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <cstdlib>

Graph GraphLoader::loadFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::string line;

    std::unordered_map<int,int> nodeMap;
    std::vector<std::pair<int,int>> edges;

    int index = 0;

    while (getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        int u, v;
        ss >> u >> v;

        if (!nodeMap.count(u)) nodeMap[u] = index++;
        if (!nodeMap.count(v)) nodeMap[v] = index++;

        edges.push_back({nodeMap[u], nodeMap[v]});
    }

    Graph graph(index);

    srand(42);

    for (auto& e : edges) {
        int w = (rand() % 10) + 1;
        graph.addEdge(e.first, e.second, w);
        graph.addEdge(e.second, e.first, w);
    }

    return graph;
}