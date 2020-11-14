#include <vector>
#include <span>
#include "utilities.h"

/**
 * Return the number of nodes of the given graph.
 */
int vertices(std::vector<int> &graph) { return graph[0]; }

/**
 * Return the number of vertices of the given graph.
 */
int edges(std::vector<int> &graph) { return graph[vertices(graph) + 1]; }

/**
 * Return the neighbours of the given vertex as a span.
 * Note that they are indexed from 1.
 *
 * @param graph The graph in the sorted representation.
 * @param vertex The vertex for which to return neighbours, indexed from 1.
 */
std::span<int> neighbours(std::vector<int> &graph, int vertex) {
    int offset = graph[vertex];
    int count = (vertex == vertices(graph) ? (int) graph.size() : graph[vertex + 1]) - graph[vertex];
    return std::span<int>(graph).subspan(offset, count);
}

/**
 * Convert the sorted representation to the pointer representation, in-place.
 */
void sorted_to_pointer(std::vector<int> &graph) {
    // non-zero-degree edges
    for (int i = vertices(graph) + 2; i < graph.size(); i++)
        if (!neighbours(graph, graph[i]).empty())
            graph[i] = graph[graph[i]];

    // zero-degree edges
    for (int i = 1; i <= vertices(graph); i++)
        if (neighbours(graph, i).empty())
            graph[i] = i;
}

/**
 * Convert the pointer representation to the swapped representation, in-place.
 */
void pointer_to_swap(std::vector<int> &graph) {
    for (int v = 1; v <= vertices(graph); v++) {
        if (v != graph[v]) {
            // A[v] = A[A[v]; A[A[v]] = v
            int tmp = graph[v];
            graph[v] = graph[graph[v]];
            graph[tmp] = v;
        }
    }
}

/**
 * Convert the swapped representation to the pointer representation, in-place.
 */
void swap_to_pointer(std::vector<int> &graph) {
    // TODO: explain that is is really important to iterate backwards!
    int v = vertices(graph);
    for (auto i = graph.size() - 1; i >= vertices(graph) + 2; i--) {
        // skip vertices of degree 0
        while (v >= 0 && graph[v] == v) v--;
        if (v < 0) break;

        if (v == graph[i]) {
            graph[i] = graph[v];
            graph[v] = i;
            v--;
        }
    }
}

/**
 * Convert the pointer representation to the sorted representation, in-place.
 */
void pointer_to_sorted(std::vector<int> &graph) {
    pointer_to_swap(graph);
    swap_to_sorted(graph);
}

/**
 * Convert the swapped representation to the sorted representation, in-place.
 */
void swap_to_sorted(std::vector<int> &graph) {
    for (int i = vertices(graph) + 2; i < graph.size(); i++)
        // n < A[i] (non-zero-degree vertices)
        if (vertices(graph) < graph[i])
            graph[i] = graph[graph[i]];

    for (int i = 1; i < vertices(graph) + 1; i++)
        graph[i] = graph[graph[i]];

    swap_to_pointer(graph);

    // restore vertices of degree 0
    // TODO: explain that is is really important to iterate backwards!
    int i_hat = graph.size();
    for (int i = vertices(graph); i >= 1; i--) {
        if (graph[i] == i) graph[i] = i_hat;
        else i_hat = graph[i];
    }
}