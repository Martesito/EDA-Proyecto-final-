#include "Graph.hpp"

Graph::Graph(int n) {
    adjList.resize(n);
}

void Graph::addEdge(int u, int v, int weight) {
    adjList[u].push_back({v, weight});
}

const std::vector<std::pair<int,int>>& Graph::getNeighbors(int u) const {
    return adjList[u];
}

int Graph::getSize() const {
    return adjList.size();
}