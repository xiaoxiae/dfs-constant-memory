#pragma once

#include <vector>
#include <span>

/**
 * Return the number of nodes of the given graph.
 */
int vertices(std::vector<int> &graph);

/**
 * Return the number of vertices of the given graph.
 */
int edges(std::vector<int> &graph);

/**
 * Return the neighbours of the given vertex as a span.
 * Note that they are indexed from 1.
 *
 * @param graph The graph in the sorted representation.
 * @param vertex The vertex for which to return neighbours, indexed from 1.
 */
std::span<int> neighbours(std::vector<int> &graph, int vertex);

/**
 * Return a random number in the given range. The last value is not included.
 * Has undefined behavior if lo >= hi.
 */
int random(int lo, int hi);

/**
 * Convert the sorted representation to the pointer representation, in-place.
 */
void sorted_to_pointer(std::vector<int> &graph);

/**
 * Convert the pointer representation to the sorted representation, in-place.
 */
void pointer_to_sorted(std::vector<int> &graph);

/**
 * Convert the pointer representation to the swapped representation, in-place.
 */
void pointer_to_swap(std::vector<int> &graph);

/**
 * Convert the swapped representation to the pointer representation, in-place.
 */
void swap_to_pointer(std::vector<int> &graph);

/**
 * Convert the swapped representation to the sorted representation, in-place.
 */
void swap_to_sorted(std::vector<int> &graph);

