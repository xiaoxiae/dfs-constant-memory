#include <vector>
#include <set>

using namespace std;

/**
 * A function that generates and returns a graph in the standard representation.
 *
 * @param n The number of vertices.
 * @param m The number of edges.
 * @param forbidden_degrees Nodes with the degrees from the vector will not be generated.
 */
vector<int> generate_graph(int n, int m, const set<int>& forbidden_degrees = set<int>()) {
    return vector<int>({5, 7, 9, 12, 14, 17, 12, 2, 5, 1, 3, 4, 2, 4, 2, 3, 5, 1, 4});

    // TODO: the actual implementation
}
