#include <vector>
#include <set>
#include "utilities.h"

/**
 * A function that generates and returns a graph in the sorted representation.
 * It generates a graph with the exact number of vertices and edges specified, and is quite slow.
 * TODO: ensure that there are no forbidden degrees
 *
 * @param n The number of vertices.
 * @param m The number of edges. $m <= n(n - 1)/2$.
 * @param forbidden_degrees Nodes with the degrees from the vector will not be generated.
 */
std::vector<int>
generate_graph(int n, int m, const std::set<int> &forbidden_degrees = std::set<int>(), bool allow_loops = false) {
    std::vector<int> result(n + m + 2, 0);
    result[0] = n;
    result[n + 1] = m;

    // set all degrees to be average
    std::vector<int> degrees(n);
    int average = m / n;
    int remaining = m % n;
    for (int i = 0; i < degrees.size(); ++i)
        degrees[i] = average + (i < remaining ? 1 : 0);

    // randomly shuffle degrees about (m^2 times - that seems about enough to be random)
    // only do so if this wouldn't create a vertex of a forbidden degree
    for (int i = 0; i < m; ++i) {
        int v1 = random(0, n);  // vertex to take from
        int v2 = random(0, n);  // vertex to add to

        // take only up to everything from v1, but not too much, since we only have so many edges (n-1, to be exact)
        int delta = random(0, std::min(degrees[v1] + 1, n - degrees[v2]));

        if (forbidden_degrees.contains(degrees[v1] - delta) || forbidden_degrees.contains(degrees[v2] + delta))
            continue;

        degrees[v1] -= delta;
        degrees[v2] += delta;
    }

    int vertex_index = n + 2;
    for (int v = 0; v < n; v++) {
        result[v + 1] = vertex_index;

        // generate random neighbours
        // we're not working on multi-graphs, so use set to prevent adding repeated neighbours
        // TODO: make this quicker
        std::set<int> neighbours;

        // possibly prevent loops
        if (!allow_loops)
            neighbours.insert(v);

        for (int i = 0; i < degrees[v]; i++) {
            int r = random(0, n);
            while (neighbours.contains(r))
                r = (r + 1) % n;

            result[vertex_index++] = r + 1;
            neighbours.insert(r);
        }

        // sort the neighbours
        std::sort(result.begin() + (vertex_index - degrees[v]), result.begin() + vertex_index);
    }

    return result;
}