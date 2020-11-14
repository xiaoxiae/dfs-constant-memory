#include <vector>
#include "utilities.h"
#include <iostream>


template<typename Pre, typename Post>
class DFS {
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

    /**
     * Return true if the specified vertex is white, else return false.
     */
    bool isWhite(int v) {
        return (v != v_s) && (1 <= A[T[v]] && A[T[v]] <= n);
    }

    /**
     * Iterate backwards from index p and return the index of the start of the adjacency array.
     */
    int iterate_backwards(int p) {
        while (A[p] > n) p--;
        return p;
    }

    /**
     * Run the DFS on the graph.
     */
    void run() {
        // variables for transferring states
        int p;
        bool is_first;

        // find the position of the starting vertex and start the DFS
        for (p = n + 2; p < n + m + 2; p++)
            if (A[p] == v_s)
                goto visit;

        /**
         * Call preprocess and visit the next neighbour.
         */
        visit:;  // int p
        {
            preprocess(A[p]);
            is_first = true;
            goto nextNeighbor;
        }

        /**
         * Main logic for changing vertices.
         * is_first prevents access for non-existent indexes (p >= n + m + 2)
         */
        nextNeighbor:;  // int p, bool is_first
        {
            // if it's the first index
            if (is_first && A[p] <= n) {
                int v = A[p];
                p++;

                // swap the first and the second
                // the first is stored in the adjacency array of the predecessor
                if (v == v_s) std::swap(A[T[p - 1]], A[p]);
                else std::swap(A[A[T[p - 1]]], A[p]);

                goto follow;
            }

                // if it's the second index
            else if (A[p - 2] <= n) {
                int v = A[p - 2];

                // if they're switched
                if ((v == v_s && A[T[p - 2]] > A[p - 1]) || A[A[T[p - 2]]] > A[p - 1]) {
                    p--;

                    if (v == v_s) std::swap(A[T[p - 1]], A[p]);
                    else std::swap(A[A[T[p - 1]]], A[p]);

                    goto follow;
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
                    postprocess(v);
                    goto restore;
                } else {
                    p = q;
                    goto backtrack;
                }
            }

            // attempt to follow pointer at p
            goto follow;
        }

        /**
         * Follow the pointer stored at p, if it's white; else go to neighbour.
         */
        follow:;
        {
            if (isWhite(A[A[p]])) {
                int q = A[p];  // where it points
                int v = A[q];  // the name of the vertex it points to

                // reverse pointer creation
                A[p] = T[v];
                T[v] = p;

                p = q;
                goto visit;
            } else {
                p++;
                is_first = false;
                goto nextNeighbor;
            }
        }

        /**
         * Backtrack from position q of vertex A[q].
         */
        backtrack:;
        {
            int v = A[p];  // name of the vertex we're backtracking from
            int r = T[v];  // reverse pointer stored at the predecessor

            // revert inverse pointer
            T[v] = A[r] + 1;
            A[r] = p;

            postprocess(v);
            p = r + 1;
            is_first = false;
            goto nextNeighbor;
        }

        /**
         * Restore the representation.
         */
        restore:;
        {
            for (int v = 1; v < n + 1; v++)
                T[v] -= 1;
        }
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
