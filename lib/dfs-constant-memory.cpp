#include <vector>
#include "utilities.h"
#include <iostream>


template<typename Pre, typename Post>
class DFS {
    // TODO: make non-recursive
    // TODO: - probably a single function with gotos, since everything is tail recursion

    std::vector<int> &graph, &T, &A;
    Pre &preprocess;
    Post &postprocess;

    int n, m, v_s;

public:
    DFS(std::vector<int> &_graph, int _v_s, Pre &_preprocess, Post &_postprocess)
            : graph(_graph), T(_graph), A(_graph), preprocess(_preprocess), postprocess(_postprocess) {
        n = vertices(graph);
        m = edges(graph);
        v_s = _v_s;
    }

    bool isWhite(int v) {
        // return true if a vertex is white
        return (v != v_s) && (1 <= A[T[v]] && A[T[v]] <= n);
    }

    void run() {
        // find the position of the starting vertex
        for (int p = n + 2; p < n + m + 2; p++)
            if (A[p] == v_s)
                visit(p);
    }

    void restore() {
        // restore the representation
        for (int v = 1; v < n + 1; v++)
            T[v] -= 1;
    }

    int iterate_backwards(int p) {
        // iterate backwards from index p until we reach another adjacency array
        if (A[p] <= n) return p;
        return iterate_backwards(p - 1);
    }

    void visit(int p) {
        // call preprocess and visit the next neighbour
        preprocess(A[p]);
        nextNeighbor(p, true);
    }

    void nextNeighbor(int p, bool is_first) {
        // main logic for changing vertices
        // is_first prevents access for non-existent indexes (p >= n + m + 2)
        // TODO: ^ what?

        // if it's the first index
        if (is_first && A[p] <= n) {
            int v = A[p];
            p += 1;

            // swap the first and the second
            // the first is stored in the adjacency array of the predecessor
            if (v == v_s) std::swap(A[T[p - 1]], A[p]);
            else std::swap(A[A[T[p - 1]]], A[p]);

            follow(p);
        }

            // if it's the second index
        else if (A[p - 2] <= n) {
            int v = A[p - 2];

            // if they're switched
            if ((v == v_s && A[T[p - 2]] > A[p - 1]) || A[A[T[p - 2]]] > A[p - 1]) {
                p--;
                if (v == v_s) std::swap(A[T[p - 1]], A[p]);
                else std::swap(A[A[T[p - 1]]], A[p]);
                follow(p);
            }
        }

        // if we went through all the neighbours
        if (p >= n + m + 2 || A[p] <= n) {
            // find the name of the vertex that we're currently iterating
            int q = iterate_backwards(p - 1);
            int v = A[q];

            // if it's the starting one then we're done
            // else backtrack
            if (v == v_s) {
                T[v]++;
                restore();
                return;
            } else {
                backtrack(q);
            }
        }

        // attempt to follow pointer at p
        follow(p);
    }

    void follow(int p) {
        // follow the pointer stored at p, if it's white; else go to neighbour
        if (isWhite(A[A[p]])) {
            int q = A[p];  // where it points
            int v = A[q];  // the name of the vertex it points to

            // reverse pointer creation
            A[p] = T[v];
            T[v] = p;

            visit(q);
        } else {
            nextNeighbor(p + 1, false);
        }
    }

    void backtrack(int q) {
        // backtrack from position q of vertex A[q]
        int v = A[q];  // name of the vertex we're backtracking from
        int p = T[v];  // reverse pointer stored at the predecessor

        // revert inverse pointer
        T[v] = A[p] + 1;
        A[p] = q;

        postprocess(v);
        nextNeighbor(p + 1, false);
    }
};

/**
 * Run DFS on the provided graph.
 *
 * @param graph The graph in the sorted representation.
 * @param start The starting vertex (indexed from 0).
 * @param preprocess A custom user function that is called each time a vertex is opened.
 * @param postprocess A custom user function that is called each time a vertex is closed.
 */
template<typename Pre, typename Post>
void dfs_constant_memory(std::vector<int> &graph, int start, Pre &preprocess, Post &postprocess) {
    sorted_to_pointer(graph);
    pointer_to_swap(graph);

    DFS<Pre, Post> dfs(graph, start + 1, preprocess, postprocess);
    dfs.run();

    swap_to_pointer(graph);
    pointer_to_sorted(graph);
}
