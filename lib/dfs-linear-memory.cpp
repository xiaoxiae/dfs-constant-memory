#include <vector>
#include <stack>
#include "utilities.h"
#include "dfs-linear-memory.h"

enum state {
    unexplored, explored
};

/**
 * Run DFS on the provided graph.
 *
 * @param graph The graph in the sorted representation.
 * @param start The starting vertex.
 * @param preprocess A custom user function that is called each time a vertex is opened.
 * @param postprocess A custom user function that is called each time a vertex is closed.
 */
template<typename Pre, typename Post>
void dfs_linear_memory(std::vector<int> &graph, int start, Pre preprocess, Post postprocess) {
    // initialize all vertices to unexplored and the starting one to open
    std::vector<state> states(vertices(graph), unexplored);
    states[start] = explored;
    dfs_linear_memory(graph, start, preprocess, postprocess, states);
}

/**
 * The internal DFS implementation using recursion.
 */
template<typename Pre, typename Post>
void
dfs_linear_memory(std::vector<int> &graph, int current, Pre preprocess, Post postprocess, std::vector<state> &states) {
    preprocess(current);
    states[current] = explored;
    for (auto &&other : neighbours(graph, current + 1)) {
        if (states[other - 1] == unexplored) {
            states[other - 1] = explored;
            dfs_linear_memory(graph, other - 1, preprocess, postprocess, states);
        }
    }
    postprocess(current);
}
