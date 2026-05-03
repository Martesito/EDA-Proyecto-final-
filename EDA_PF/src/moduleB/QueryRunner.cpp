#include "QueryRunner.hpp"
#include "Dijkstra.hpp"
#include "BFS.hpp"
#include "../utils/Timer.hpp"
#include <fstream>
#include <vector>
#include <climits>

void QueryRunner::runAll(const Graph& graph) {
    std::vector<std::pair<std::string, std::pair<int,int>>> queries = {
        {"Q01",{1,500000}},
        {"Q02",{100,1000000}},
        {"Q03",{50000,750000}},
        {"Q04",{200000,800000}},
        {"Q05",{300000,100000}},
        {"Q06",{1,1087562}},
        {"Q07",{500000,1}},
        {"Q08",{250000,600000}},
        {"Q09",{10000,900000}},
        {"Q10",{400000,150000}}
    };

    std::ofstream file("results/consultas_p2p.csv");

    file << "query,source,target,distDijkstra,distBfs,nodesDijkstra,nodesBfs,timeDijkstra,timeBfs\n";

    for (auto& q : queries) {
        std::string name = q.first;
        int source = q.second.first;
        int target = q.second.second;

        Result dRes, bRes;

        double tDijkstra = Timer::measure([&]() {
            dRes = Dijkstra::run(graph, source, target);
        });

        double tBfs = Timer::measure([&]() {
            bRes = BFS::run(graph, source, target);
        });

        file << name << ","
             << source << ","
             << target << ","
             << (dRes.distance == INT_MAX ? -1 : dRes.distance) << ","
             << (bRes.distance == INT_MAX ? -1 : bRes.distance) << ","
             << dRes.nodesExplored << ","
             << bRes.nodesExplored << ","
             << tDijkstra << ","
             << tBfs << "\n";
    }

    file.close();
}