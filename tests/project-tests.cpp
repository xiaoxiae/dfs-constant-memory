#include "../lib/generator.h"
#include "../lib/dfs-linear-memory.cpp"
#include "gtest/gtest.h"
#include <queue>
#include <stack>
#include <vector>


// for running larger tests
// too slow for development
#define LARGE_TESTS 1

// how many graphs of each type to generate and test against
constexpr int GENERATIONS = 100;

/**
 * A wrapper on random() from utilities that is seeded.
 */
int random_seeded(int lo, int hi) {
    static bool seeded = false;
    if (!seeded) { seeded = true; }

    return random(lo, hi);
}

/**
 * Attach a formatted graph representation to a debug message.
 */
std::string attach_graph(std::string m, const std::vector<int>& graph) {
    m = m + " [";
    int i = 0;
    for (; i < graph.size() - 1; ++i)
        m += std::to_string(graph[i]) + ", ";
    return m + std::to_string(graph[i]) + "]";
}

/**
 * Check the correctness of the graph, given in the sorted standard representation.
 */
void check_graph_correctness(std::vector<int>& graph, int n, int m, const std::set<int>& forbidden_degrees = std::set<int>()) {
    ASSERT_EQ(vertices(graph), n)
    << attach_graph("Number of vertices in the generated graph doesn't match the wanted number.", graph);

    ASSERT_EQ(edges(graph), m)
    << attach_graph("Number of edges in the generated graph doesn't match the wanted number.", graph);

    ASSERT_EQ(graph.size(), n + m + 2)
    << attach_graph("The graph has an incorrect size.", graph);

    // check that the graph doesn't contain forbidden degrees
    for (int v = 0; v < n; v++)
        ASSERT_FALSE(forbidden_degrees.contains(neighbours(graph, v).size()))
        << attach_graph("Vertex " + std::to_string(v) + " has a forbidden degree " + std::to_string(neighbours(graph, v).size()) + ":", graph);

    // check that the vertices of the graph are sorted
    for (int v = 0; v < n - 1; v++)
        ASSERT_TRUE(graph[v + 1] <= graph[v + 2])
        << attach_graph("Vertices " + std::to_string(v + 1) + " has a forbidden degree:", graph);

    // check that each neighbour value is in a valid range
    for (int v = n + 2; v < n + m + 2; v++)
        ASSERT_TRUE(1 <= graph[v] && graph[v] <= n);

    // check that the vertices point to reasonable places
    for (int v = 0; v < n; v++)
        ASSERT_TRUE(n + 2 <= graph[v + 1] && graph[v + 1] <= n + m + 2)
        << attach_graph("Vertex " + std::to_string(v + 1) + " points to an incorrect index:", graph);

    // check that neighbours of each vertex are sorted
    for (int v = 0; v < n; v++) {
        auto nb = neighbours(graph, v);

        // care! nb.size() - 1 is unsigned, so 0 - 1 is gigantic
        for (int i = 1; i < nb.size(); i++) {
            ASSERT_TRUE(nb[i - 1] < nb[i])
            << attach_graph(
                "The neighbours " + std::to_string(nb[i - 1]) + " and " + std::to_string(nb[i]) +
                " of vertex " + std::to_string(v) +
                " are not sorted:", graph
            );
        }
    }
}

/**
 * Check graphs of the given sizes.
 */
std::vector<int> generate_random_graph(int n_lo, int n_hi) {
    int n = random_seeded(n_lo, n_hi);

    // random(0, 0) has undefined behavior, because hi si not included
    // a complete directed graph has n(n - 1) edges.
    int m = (n == 1) ? 0 : random_seeded(0, n * (n - 1) + 1);

    return generate_graph(n, m);
}

/**
 * Test the correct generation of graphs with random parameters.
 */
void test_graph_generation(int n_lo, int n_hi) {
    for (int i = 0; i < GENERATIONS; ++i) {
        auto graph = generate_random_graph(n_lo, n_hi);
        check_graph_correctness(graph, vertices(graph), edges(graph));
    }
}

TEST(ArrayTestSuite, GenerateSmall){ test_graph_generation(1, 10); }
TEST(ArrayTestSuite, GenerateMedium){ test_graph_generation(10, 100); }
#if LARGE_TESTS
TEST(ArrayTestSuite, GenerateLarge){ test_graph_generation(100, 1000); }
#endif

// TODO: tests including the forbidden edges

/**
/**
 * Check the correctness of the order.
 * Positive values in the vector signal a calls to preprocess, negative to postprocess.
 *
 * @param graph The graph to check DFS order on.
 * @param order A vector of values where +v means "entered a vertex" and -v means "left it". Indexed from 1.
 * @param start The starting vertex. Indexed from 1.
 */
void check_dfs_order(std::vector<int>& graph, std::vector<int> order, int start) {
    // find all accessible vertices from the start using BFS
    std::vector<bool> explored(vertices(graph));
    std::queue<int> queue;
    queue.push(start - 1);
    explored[start - 1] = true;
    while (!queue.empty())  {
        int current = queue.front();
        queue.pop();

        for(int neighbour: neighbours(graph, current)) {
            if (!explored[neighbour - 1]) {
                explored[neighbour - 1] = true;
                queue.push(neighbour - 1);
            }
        }
    }

    // check that all of them have been explored
    for (int i : order) explored[abs(i) - 1] = false;
    for (int i = 0; i < explored.size(); ++i)
        ASSERT_TRUE(!explored[i])
        << attach_graph("The vertex " + std::to_string(i) + " is accessible but was not explored.", graph);

    ASSERT_EQ(order[0], start) << attach_graph("The starting vector is not entered first.", graph);
    ASSERT_EQ(order[order.size() - 1], -start) << attach_graph("The starting vector is not exited last.", graph);

    // simulate the DF
    std::stack<int> path;
    path.push(start);

    for (int v : order) {
        if (abs(v) == start) continue;

        // if we're appending
        if (v > 0) {
            auto nb = neighbours(graph, path.top() - 1);

            // check if we can go to that vertex
            ASSERT_TRUE(std::find(nb.begin(), nb.end(), v) != nb.end())
            << attach_graph("Vertex " + std::to_string(path.top()) + " does not contain vertex " + std::to_string(v) + " as a neighbour.", graph);

            path.push(v);
        } else {
            ASSERT_EQ(path.top(), abs(v))
            << attach_graph("Postprocess was called on vertex " + std::to_string(abs(v)) + ", although we're in " + std::to_string(path.top()) + ":", graph);

            path.pop();
        }
    }

    // check that no vertex is either both entered and visited or neither
    // done by checking whether both preprocess and postprocess is called
    // done last, since it messes with the array
    std::sort(order.begin(), order.end(), [](int a, int b) {return abs(a) < abs(b);});
    for (int i = 0; i < order.size(); i += 2)
        ASSERT_EQ(order[i], -order[i + 1])
        << attach_graph("Preprocess and postprocess not called on " + std::to_string(order[i]) + ".", graph);
}

/**
 * Test the correct generation of graphs with random parameters.
 */
void test_dfs_correctness(int n_lo, int n_hi) {
    for (int i = 0; i < GENERATIONS; ++i) {
        std::vector<int> order;
        auto graph = generate_random_graph(n_lo, n_hi);
        int start = random_seeded(0, vertices(graph));

        // a DFS run that stores the vertex order in a vector to check for correctness
        // note that they are indexed from 1, because -0 == 0...
        dfs_linear_memory(
            graph,
            start,
            [&order] (int v) { order.push_back(v + 1); },
            [&order] (int v) { order.push_back(-v - 1); }
        );

        check_dfs_order(graph, order, start + 1);
    }
}

TEST(ArrayTestSuite, DFSLinearSmall){ test_dfs_correctness(1, 10); }
TEST(ArrayTestSuite, DFSLinearMedium){ test_dfs_correctness(10, 100); }
#if LARGE_TESTS
TEST(ArrayTestSuite, DFSLinearLarge){ test_dfs_correctness(100, 1000); }
#endif

// TODO tests for the constant-time DFS