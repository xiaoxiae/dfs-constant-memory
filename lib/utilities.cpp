#include <vector>

using namespace std;

/**
 * Return the number of nodes from the given graph.
 */
int vertices(vector<int> A) { return A[0]; }

/**
 * Return the number of vertices from the given graph.
 */
int edges(vector<int> A) { return A[vertices(A) + 1]; }

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
