#include <vector>
#include <span>
#include "utilities.h"

using namespace std;

int vertices(vector<int> graph) { return graph[0]; }

int edges(vector<int> graph) { return graph[vertices(graph) + 1]; }

span<int> neighbours(vector<int> graph, int vertex) {
    int offset = graph[vertex + 1];
    int count = (vertex + 1 == vertices(graph) ? (int)graph.size() : graph[vertex + 2]) - graph[vertex + 1];
    return span<int>(graph).subspan(offset, count);
}

int random(int lo, int hi) { return lo + rand() % (hi - lo); }

void sorted_to_pointer(vector<int> A) {
    // regular edges
    for (int i = vertices(A) + 2; i < vertices(A) + edges(A) + 2; i++) {
        A[i] = A[A[i]];
    }
}

void pointer_to_swap(vector<int> A) {
    for (int v = 1; v < vertices(A) + 1; v++) {
        if (v != A[v]) {
            int tmp = A[v];
            A[v] = A[A[v]];  // save first vertex to T
            A[tmp] = v;      // v's edges start here
        }
    }
}

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