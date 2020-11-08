#include "../lib/generator.cpp"
#include "../lib/utilities.cpp"
#include "gtest/gtest.h"
#include <cstdlib>

/**
 * Check the correctness of the graph, given in the sorted standard representation.
 */
void check_correctness(const vector<int> result, int n, int m, const vector<int>& forbidden_degrees = vector<int>()) {
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
void check_correctness(int n_lo, int n_hi, int m_lo, int m_hi, int count = 100) {
    for (int i = 0; i < count; ++i) {
        int n = rand() % (n_hi - n_lo) + n_lo;
        int m = rand() % (m_hi - m_lo) + m_lo;
        check_correctness(generate_graph(n, 5), n, m);
    }
}

TEST(ArrayTestSuite, GenerateSmall){ check_correctness(10, 100, 0, 100); }
TEST(ArrayTestSuite, GenerateMedium){ check_correctness(1000, 10000, 0, 10000); }
TEST(ArrayTestSuite, GenerateLarge){ check_correctness(100000, 1000000, 0, 1000000); }
