#pragma once
#include <vector>
#include <span>

/**
 * Return the number of nodes from the given graph.
 * @param graph The graph in any of the three representations.
 */
int vertices(std::vector<int>& graph);

/**
 * Return the number of vertices from the given graph.
 *
 * @param graph The graph in any of the three representations.
 */
int edges(std::vector<int>& graph);

/**
 * Return the neighbours of the given vertex as a span.
 * Note that they are indexed from 1, not 0 (as per the paper)!
 *
 * @param graph The graph in the sorted representation.
 * @param vertex The vertex for which to return neighbours, indexed from 0.
 */
std::span<int> neighbours(std::vector<int>& graph, int vertex);

/**
 * Return a random number in the given range. The last value is not included.
 * Has undefined behavior if lo >= hi.
 */
int random(int lo, int hi);

/**
 * Convert the sorted representation to pointer representation, in-place.
 */
void sorted_to_pointer(std::vector<int>& graph);

/**
 * Convert the sorted representation to swapped representation.
 */
void pointer_to_swap(std::vector<int>& graph);

/**
 * Convert the swapped representation to sorted represenation.
 */
void swap_to_sorted(std::vector<int>& graph);
