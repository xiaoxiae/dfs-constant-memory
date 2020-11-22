#pragma once

#include <vector>
#include "utilities.h"
#include <iostream>


template<class Pre, class Post>
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
    inline bool is_white(int v) {
        return !is_starting(v)  // starting vertex is never white
               && is_vertex(A[T[v]]);  // degree 2
    }

    /**
     * Return true if the given value can be a name of a vertex (it falls in range).
     */
    inline bool is_vertex(int v) { return 1 <= v && v <= n; }

    /**
     * Return true if the given value can be a pointer to some index in A.
     */
    inline bool is_pointer(int v) { return n + 2 <= v && v <= n + m + 2; }

    /**
     * Return true if the given value is the starting vertex.
     */
    inline bool is_starting(int v) { return v == v_s; }

    /**
     * Iterate backwards from index p and return the index of the start of the adjacency array.
     */
    inline int iterate_backwards(int p) {
        while (is_pointer(A[p])) p--;
        return p;
    }

    /**
     * Prevent visiting the first neighbour of A[p] from the first index by visiting first from the second position
     * and then swapping back. (see presentation slide 12).
     */
    inline void prevent_first_position_visit(int p) {
        // special case for first vertex - it doesn't have a reverse pointer (we don't have to follow using A[...])
        if (is_starting(A[p])) std::swap(A[T[p]], A[p + 1]);
        else std::swap(A[A[T[p]]], A[p + 1]);
    }

    /**
     * Run the DFS on the graph.
     */
    void run() {
        // variables for transferring states
        int p;          // the current vertex position
        bool is_first;  // whether it's the first neighbour of a given vertex we're visiting

        // find the position of the starting vertex and start the DFS
        for (p = n + 2; p < n + m + 2; p++)
            if (is_starting(A[p]))
                goto visit;

        /**
         * Call preprocess and visit the next neighbour.
         */
        visit: // p
        {
            preprocess(A[p] - 1);

            is_first = true;
            goto nextNeighbor;
        }

        /**
         * Main logic for changing vertices.
         */
        nextNeighbor: // p, is_first
        {
            // if we want to visit the first neighbour from the first index, don't
            // @presentation(12)
            if (is_first) {
                prevent_first_position_visit(p++);
                goto follow;
            }

            int v = A[p - 2];

            // if they're switched (from not wanting to visit the neighbour from the first index), switch them back
            // @presentation(12)
            if (is_vertex(v) &&
                ((is_starting(v) && A[T[p - 2]] > A[p - 1]) || (!is_starting(v) && A[A[T[p - 2]]] > A[p - 1]))) {
                p -= 2;
                prevent_first_position_visit(p++);

                goto follow;
            }

            // if we went through all the neighbours
            if (p >= n + m + 2 || is_vertex(A[p])) {
                // find the name of the vertex that we're currently iterating
                int q = iterate_backwards(p - 1);
                int v = A[q];

                // if it's the starting one then we're done
                if (is_starting(v)) {
                    T[v]++;
                    postprocess(v - 1);
                    goto restore;
                }

                // else backtrack
                p = q;
                goto backtrack;
            }

            // attempt to follow pointer at p
            goto follow;
        }

        /**
         * Follow the pointer stored at p, if it's white; else go to neighbour.
         */
        follow: // p
        {
            if (is_white(T[A[p]])) {
                // @presentation(11)
                // create a reverse pointer
                int q = A[p];
                int v = A[q];
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
         * Backtrack from position p of vertex A[p].
         */
        backtrack: // p
        {
            // @presentation(13) (care - the names don't match)
            int v = A[p];  // name of the vertex we're backtracking from
            int q = A[v];  // reverse pointer

            // undo the reverse pointer
            T[v] = A[q] + 1;  // +1 to mark it grey-black
            A[q] = p;
            postprocess(v - 1);
            p = q + 1;

            is_first = false;
            goto nextNeighbor;
        }

        /**
         * Restore the representation.
         */
        restore:
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
template<class Pre, class Post>
void dfs_constant_memory(std::vector<int> &graph, int start, Pre &preprocess, Post &postprocess) {
    sorted_to_pointer(graph);
    pointer_to_swap(graph);

    DFS<Pre, Post> dfs(graph, start + 1, preprocess, postprocess);
    dfs.run();

    swap_to_pointer(graph);
    pointer_to_sorted(graph);
}
