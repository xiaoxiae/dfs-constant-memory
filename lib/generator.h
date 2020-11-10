#pragma once
#include <vector>
#include <set>
#include "utilities.h"
#include "generator.h"

/**
 * A function that generates and returns a graph in the standard representation.
 *
 * @param n The number of vertices.
 * @param m The number of edges.
 * @param forbidden_degrees Nodes with the degrees from the vector will not be generated.
 */
std::vector<int> generate_graph(int n, int m, const std::set<int>& forbidden_degrees = std::set<int>());
