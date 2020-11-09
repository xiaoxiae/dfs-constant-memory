#include <vector>
#include <span>

using namespace std;

/**
 * Return the number of nodes from the given graph.
 * @param graph The graph in any of the three representations.
 */
int vertices(vector<int> graph) { return graph[0]; }

/**
 * Return the number of vertices from the given graph.
 * @param graph The graph in any of the three representations.
 */
int edges(vector<int> graph) { return graph[vertices(graph) + 1]; }

/**
 * Return the neighbours of the given vertex.
 * @param graph The graph in the sorted representation.
 * @param vertex The vertex for which to return neighbours, indexed from 0.
 */
span<int> neighbours(vector<int> graph, int vertex) {
    int offset = graph[vertex + 1];
    int count = (vertex + 1 == vertices(graph) ? (int)graph.size() : graph[vertex + 2]) - graph[vertex + 1];
    return span<int>(graph).subspan(offset, count);
}


/**
 * Convert the sorted representation to pointer representation, in-place.
 */
void sorted_to_pointer(vector<int> A) {
    // regular edges
    for (int i = vertices(A) + 2; i < vertices(A) + edges(A) + 2; i++) {
        A[i] = A[A[i]];
    }
}

/**
 * Convert the sorted representation to swapped representation.
 */
void pointer_to_swap(vector<int> A) {
    for (int v = 1; v < vertices(A) + 1; v++) {
        if (v != A[v]) {
            int tmp = A[v];
            A[v] = A[A[v]];  // save first vertex to T
            A[tmp] = v;      // v's edges start here
        }
    }
}

/**
 * Convert the sorted representation to swapped representation.
 */
void swap_to_sorted(vector<int> A) {
    for (int i = vertices(A) + 2; i < vertices(A) + edges(A) + 2; i++)
        if (vertices(A) < A[i]) A[i] = A[A[i]];

    int v = vertices(A);
    int i = vertices(A) + vertices(A) + 2;
    do {
        // skip vertices of degree 0
        while (A[v] == v && v > 0) v--;
    } while (v > 0);
}
