#include <vector>
#include <set>
#include "utilities.h"

using namespace std;

/**
 * A function that generates and returns a graph in the standard representation.
 *
 * @param n The number of vertices.
 * @param m The number of edges.
 * @param forbidden_degrees Nodes with the degrees from the vector will not be generated.
 */
vector<int> generate_graph(int n, int m, const set<int>& forbidden_degrees = set<int>()) {
    vector<int> result(n + m + 2, 0);
    result[0] = n;
    result[n + 1] = m;

    // TODO: first, randomly assign degrees to all vertices

    // set all degrees to be average
    vector<int> degrees(n);
    int average = m / n;
    int remaining = m % n;
    for (int i = 0; i < degrees.size(); ++i)
        degrees[i] = average + (i < remaining ? 1 : 0);

    // randomly shuffle degrees about (m^2 times -- that seems about enough to be random)
    // only do so if this wouldn't create
    for (int i = 0; i < m * m; ++i) {
        int v1 = random(0, n);
        int v2 = random(0, n);
        // TODO: split to header files to this isn't completely broken
    }

    // TODO: ensure that there are no forbidden degrees
    return result;
}
