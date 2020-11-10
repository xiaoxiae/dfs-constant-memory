#include "../lib/generator.h"
#include "../lib/dfs-linear-memory.cpp"
#include "gtest/gtest.h"
#include <cstdlib>

// how many graphs of each type to generate and test against
constexpr int GENERATIONS = 100;

/**
 * A wrapper on random() from utils that is seeded.
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
void check_graph_correctness(const std::vector<int>& graph, int n, int m, const std::set<int>& forbidden_degrees = std::set<int>()) {
    ASSERT_EQ(vertices(graph), n)
    << attach_graph("Number of vertices in the generated graph doesn't match the wanted number.", graph);

    ASSERT_EQ(edges(graph), m)
    << attach_graph("Number of edges in the generated graph doesn't match the wanted number.", graph);

    ASSERT_EQ(graph.size(), n + m + 2)
    << attach_graph("The graph has an incorrect size.", graph);

    // check that the graph doesn't contain forbidden degrees
    for (int v = 0; v < vertices(graph); v++)
        ASSERT_FALSE(forbidden_degrees.contains(neighbours(graph, v).size()))
        << attach_graph("Vertex " + std::to_string(v) + " has a forbidden degree:", graph);

    // check that neighbours of each vertex are sorted
    for (int v = 0; v < vertices(graph); v++) {
        auto nb = neighbours(graph, v);
        for (int i = 0; i < nb.size() - 1; i++)
            ASSERT_TRUE(nb[i] < nb[i + 1])
            << attach_graph(
                "The neighbours " + std::to_string(nb[i]) + " and " + std::to_string(nb[i + 1]) +
                " of vertex " + std::to_string(v) +
                " are not sorted:", graph
            );
    }
}

/**
 * Check graphs of the given sizes.
 */
std::vector<int> generate_random_graph(int n_lo, int n_hi, int m_lo, int m_hi) {
    int n = random_seeded(n_lo, n_hi);
    int m = random_seeded(m_lo, m_hi);
    return generate_graph(n, m);
}

/**
 * Test the correct generation of graphs with random parameters.
 */
void test_graph_generation(int n_lo, int n_hi, int m_lo, int m_hi) {
    for (int i = 0; i < GENERATIONS; ++i) {
        auto graph = generate_random_graph(n_lo, n_hi, m_lo, m_hi);
        check_graph_correctness(graph, vertices(graph), edges(graph));
    }
}

TEST(ArrayTestSuite, GenerateSmall){ test_graph_generation(10, 100, 0, 100); }
TEST(ArrayTestSuite, DISABLED_GenerateMedium){ test_graph_generation(1000, 10000, 0, 10000); }
TEST(ArrayTestSuite, DISABLED_GenerateLarge){ test_graph_generation(100000, 1000000, 0, 1000000); }
// TODO: tests including the forbidden edges

/**
 * Check the correctness of the order.
 * Positive values in the vector signal a calls to preprocess, negative to postprocess.
 */
void check_dfs_order(const std::vector<int>& graph, std::vector<int> order) {
    // TODO: check order
    // TODO: check that all accessible edges have been explored

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
void test_dfs_correctness(int n_lo, int n_hi, int m_lo, int m_hi) {
    for (int i = 0; i < GENERATIONS; ++i) {
        std::vector<int> order;
        auto graph = generate_random_graph(n_lo, n_hi, m_lo, m_hi);
        int start = random_seeded(0, vertices(graph));

        // a DFS run that stores the vertex order in a vector to check for correctness
        dfs_linear_memory(
            graph,
            start,
            [&order] (int v) { order.push_back(v); },
            [&order] (int v) { order.push_back(-v); }
        );

        check_dfs_order(graph, order);
    }
}

TEST(ArrayTestSuite, DISABLED_DFSLinearSmall){ test_dfs_correctness(10, 100, 0, 100); }
TEST(ArrayTestSuite, DISABLED_DFSLinearMedium){ test_dfs_correctness(1000, 10000, 0, 10000); }
TEST(ArrayTestSuite, DISABLED_DFSLinearLarge){ test_dfs_correctness(100000, 1000000, 0, 1000000); }

// TODO tests for the constant-time DFS