#pragma once

#include <vector>
#include <span>


/**
 * Return the number of nodes of the given graph.
 */
inline int vertices(std::vector<int> &graph) { return graph[0]; }

/**
 * Return the number of vertices of the given graph.
 */
inline int edges(std::vector<int> &graph) { return graph[vertices(graph) + 1]; }

std::span<int> neighbours(std::vector<int> &graph, int vertex);

void sorted_to_pointer(std::vector<int> &graph);

void pointer_to_sorted(std::vector<int> &graph);

void pointer_to_swap(std::vector<int> &graph);

void swap_to_pointer(std::vector<int> &graph);

void swap_to_sorted(std::vector<int> &graph);

