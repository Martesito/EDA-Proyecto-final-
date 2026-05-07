#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <vector>
#include <unordered_map>

struct Edge {
    int to;
    int weight;
};

class Graph {
private:
    std::vector<std::vector<Edge>> adj;
    std::unordered_map<int, int> orig_to_internal;
    std::vector<int> internal_to_orig;
    int n_nodes;
    long long n_edges;


