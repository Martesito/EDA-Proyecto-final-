#pragma once
#include <vector>
#include <utility>

class Graph {
private:
    std::vector<std::vector<std::pair<int,int>>> adjList;

public:
    Graph(int n);

    void addEdge(int u, int v, int weight);

    const std::vector<std::pair<int,int>>& getNeighbors(int u) const;

    int getSize() const;
};