#include <vector>
#include <stack>
#include "utilities.h"
#include "dfs-linear-memory.h"

enum state{unexplored, open, closed};

template <typename Pre, typename Post>
void dfs_linear_memory(std::vector<int>& graph, int start, Pre preprocess, Post postprocess) {
    // initialize all vertices to unexplored and the starting one to open
    std::vector<state> states(vertices(graph), unexplored);
    states[start] = open;
    dfs_linear_memory(graph, start, preprocess, postprocess, states);
}

/**
 * The internal DFS implementation using recursion.
 */
template <typename Pre, typename Post>
void dfs_linear_memory(std::vector<int>& graph, int current, Pre preprocess, Post postprocess, std::vector<state>& states) {
    preprocess(current);
    states[current] = open;
    for (auto &&other : neighbours(graph, current)) {
        if (states[other - 1] == unexplored) {
            states[other - 1] = open;
            dfs_linear_memory(graph, other - 1, preprocess, postprocess, states);
            states[other - 1] = closed;
        }
    }
    postprocess(current);
}