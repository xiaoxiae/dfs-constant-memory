#include "../lib/generator.cpp"
#include "../lib/dfs-linear-memory.cpp"
#include "gtest/gtest.h"
#include <cstdlib>

// how many graphs of each type to generate and test against
constexpr int GENERATIONS = 100;


/**
 * Return a random number in the given range.
 */
int random(int lo, int hi) {
    static bool seeded = false;
    if (!seeded) {srand(0xbdeadbeef); seeded = true; }

    return lo + rand() % (hi - lo); }


/**
 * Check the correctness of the graph, given in the sorted standard representation.
 */
void check_graph_correctness(const vector<int>& result, int n, int m, const vector<int>& forbidden_degrees = vector<int>()) {
    ASSERT_EQ(vertices(result), n);
    ASSERT_EQ(edges(result), m);
    ASSERT_EQ(result.size(), n + m + 2);  // vertices + edges + their numbers

    // TODO: test forbidden edges
    // TODO: test sorted representation
    // TODO: test correct number of vertices
}

/**
 * Check graphs of the given sizes.
 */
vector<int> generate_random_graph(int n_lo, int n_hi, int m_lo, int m_hi) {
    int n = random(n_lo, n_hi);
    int m = random(m_lo, m_hi);;
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
// TODO: tests for forbidden edges

/**
 * Check the correctness of the order.
 * Positive values in the vector signal a calls to preprocess, negative to postprocess.
 */
void check_dfs_order(const vector<int>& result, const vector<int>& order) {
    // TODO: order (whether we can always get to what it says we're doing)
    // TODO: no vertices visited twice
}

/**
 * Test the correct generation of graphs with random parameters.
 */
void test_dfs_correctness(int n_lo, int n_hi, int m_lo, int m_hi) {
    for (int i = 0; i < GENERATIONS; ++i) {
        vector<int> operations;
        auto graph = generate_random_graph(n_lo, n_hi, m_lo, m_hi);
        int start = random(n_lo, n_hi);

        // a DFS run that stores the vertex order in a vector to check for correctness
        dfs_linear_memory(
                graph,
                start,
                [&operations] (int v) { operations.push_back(v);},
                [&operations] (int v) { operations.push_back(-v);}
        );

        check_dfs_order(graph, operations);
    }
}

TEST(ArrayTestSuite, DFSLinearSmall){ test_dfs_correctness(10, 100, 0, 100); }
TEST(ArrayTestSuite, DISABLED_DFSLinearMedium){ test_dfs_correctness(1000, 10000, 0, 10000); }
TEST(ArrayTestSuite, DISABLED_DFSLinearLarge){ test_dfs_correctness(100000, 1000000, 0, 1000000); }

// TODO tests for the constant-time DFS