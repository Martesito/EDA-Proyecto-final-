#include "Dijkstra.hpp"
#include <queue>
#include <limits>
#include <algorithm>

Result Dijkstra::run(const Graph& graph, int source, int target) {
    int n = graph.getSize();
    const int INF = std::numeric_limits<int>::max();

    std::vector<int> dist(n, INF);
    std::vector<int> prev(n, -1);
    std::vector<bool> visited(n, false);

    std::priority_queue<
        std::pair<int,int>,
        std::vector<std::pair<int,int>>,
        std::greater<>
    > pq;

    dist[source] = 0;
    pq.push({0, source});

    int nodesExplored = 0;

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (visited[u]) continue;
        visited[u] = true;

        nodesExplored++;

        if (u == target) break;

        for (auto [v, w] : graph.getNeighbors(u)) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                prev[v] = u;
                pq.push({dist[v], v});
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