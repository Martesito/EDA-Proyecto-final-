#include "BFS.hpp"
#include <queue>
#include <limits>
#include <algorithm>

Result BFS::run(const Graph& graph, int source, int target) {
    int n = graph.getSize();
    const int INF = std::numeric_limits<int>::max();

    std::vector<int> dist(n, INF);
    std::vector<int> prev(n, -1);

    std::queue<int> q;
    dist[source] = 0;
    q.push(source);

    int nodesExplored = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        nodesExplored++;

        if (u == target) break;

        for (auto [v, _] : graph.getNeighbors(u)) {
            if (dist[v] == INF) {
                dist[v] = dist[u] + 1;
                prev[v] = u;
                q.push(v);
            }
        }
    }

    Result res;
    res.distance = dist[target];
    res.nodesExplored = nodesExplored;

    if (dist[target] != INF) {
        for (int at = target; at != -1; at = prev[at])
            res.path.push_back(at);

        std::reverse(res.path.begin(), res.path.end());
    }

    return res;
}