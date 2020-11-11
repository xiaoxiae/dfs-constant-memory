#include <vector>
#include <span>
#include "utilities.h"

int vertices(std::vector<int>& graph) { return graph[0]; }

int edges(std::vector<int>& graph) { return graph[vertices(graph) + 1]; }

std::span<int> neighbours(std::vector<int>& graph, int vertex) {
    int offset = graph[vertex + 1];
    int count = (vertex + 1 == vertices(graph) ? (int)graph.size() : graph[vertex + 2]) - graph[vertex + 1];
    return std::span<int>(graph).subspan(offset, count);
}

int random(int lo, int hi) { return lo + rand() % (hi - lo); }

void sorted_to_pointer(std::vector<int>& graph) {
    // regular edges
    for (int i = vertices(graph) + 2; i < vertices(graph) + edges(graph) + 2; i++) {
        graph[i] = graph[graph[i]];
    }
}

void pointer_to_swap(std::vector<int>& graph) {
    for (int v = 1; v < vertices(graph) + 1; v++) {
        if (v != graph[v]) {
            int tmp = graph[v];
            graph[v] = graph[graph[v]];  // save first vertex to T
            graph[tmp] = v;      // v's edges start here
        }
    }
}

void swap_to_sorted(std::vector<int>& graph) {
    for (int i = vertices(graph) + 2; i < vertices(graph) + edges(graph) + 2; i++)
        if (vertices(graph) < graph[i]) graph[i] = graph[graph[i]];

    int v = vertices(graph);
    int i = vertices(graph) + vertices(graph) + 2;
    do {
        // skip vertices of degree 0
        while (graph[v] == v && v > 0) v--;
    } while (v > 0);
}