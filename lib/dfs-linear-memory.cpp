#include <vector>
#include <stack>
#include "utilities.cpp"

using namespace std;
enum state{unexplored, open, closed};

/**
 *
 * @param graph The graph in the sorted representation.
 * @param start The starting vertex.
 * @param preprocess A custom user function that is called each time a vertex is opened.
 * @param postprocess A custom user function that is called each time a vertex is closed.
 */
template <typename Pre, typename Post>
void dfs_linear_memory(vector<int>& graph, int start, Pre preprocess, Post postprocess) {
    // initialize all vertices to unexplored and the starting one to open
    vector<state> states(vertices(graph), unexplored);
    states[start] = open;
    dfs_linear_memory(graph, start, preprocess, postprocess, states);
}

template <typename Pre, typename Post>
void dfs_linear_memory(vector<int>& graph, int current, Pre preprocess, Post postprocess, vector<state>& states) {
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
