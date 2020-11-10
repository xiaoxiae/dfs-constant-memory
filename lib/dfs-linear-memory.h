#pragma once
#include <vector>

/**
 * Run DFS on the provided graph.
 *
 * @param graph The graph in the sorted representation.
 * @param start The starting vertex.
 * @param preprocess A custom user function that is called each time a vertex is opened.
 * @param postprocess A custom user function that is called each time a vertex is closed.
 */
template <typename Pre, typename Post>
void dfs_linear_memory(std::vector<int>& graph, int start, Pre preprocess, Post postprocess);
