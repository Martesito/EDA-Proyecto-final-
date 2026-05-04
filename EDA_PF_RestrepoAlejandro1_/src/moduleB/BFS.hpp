#pragma once
#include "../graph/Graph.hpp"
#include "Result.hpp"

class BFS {
public:
    static Result run(const Graph& graph, int source, int target);
};