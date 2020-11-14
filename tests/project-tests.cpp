#include "../lib/dfs-linear-memory.cpp"
#include "../lib/dfs-constant-memory.cpp"
#include "gtest/gtest.h"
#include <queue>
#include <stack>
#include <vector>

// for running larger tests
// too slow for development
#define LARGE_TESTS 0

#define SMALL 3, 10
#define MEDIUM 10, 100
#define LARGE 100, 1000

// how many graphs of each type to generate and test against
constexpr int GENERATIONS = 100;

/**
 * Return a random number in the given range. The last value is not included.
 * Has undefined behavior if lo >= hi.
 */
int random(int lo, int hi) {
    static bool seeded = false;
    if (!seeded) { seeded = true; }

    return lo + rand() % (hi - lo);
}

/**
 * A function that generates and returns a graph in the sorted representation.
 * It generates a graph with the exact number of vertices and edges specified, and is quite slow.
 *
 * @param n The number of vertices.
 * @param m The number of edges. $m <= n(n - 1)/2$.
 * @param loops Whether loops are allowed.
 */
std::vector<int>
generate_graph(int n, int m, bool loops = false) {
    std::vector<int> result(n + m + 2, 0);
    result[0] = n;
    result[n + 1] = m;

    // set all degrees to be average
    std::vector<int> degrees(n);
    int average = m / n;
    int remaining = m % n;
    for (int i = 0; i < degrees.size(); ++i)
        degrees[i] = average + (i < remaining ? 1 : 0);

    // randomly shuffle degrees about (m^2 times - that seems about enough to be random)
    // only do so if this wouldn't create a vertex of a forbidden degree
    for (int i = 0; i < m; ++i) {
        int v1 = random(0, n);  // vertex to take from
        int v2 = random(0, n);  // vertex to add to

        // take only up to everything from v1, but not too much, since we only have so many edges (n-1, to be exact)
        int delta = random(0, std::min(degrees[v1] + 1, n - degrees[v2]));

        degrees[v1] -= delta;
        degrees[v2] += delta;
    }

    int vertex_index = n + 2;
    for (int v = 0; v < n; v++) {
        result[v + 1] = vertex_index;

        // generate random neighbours
        // we're not working on multi-graphs, so use set to prevent adding repeated neighbours
        std::set<int> neighbours;

        // possibly prevent loops
        if (!loops)
            neighbours.insert(v);

        for (int i = 0; i < degrees[v]; i++) {
            int r = random(0, n);
            while (neighbours.contains(r))
                r = (r + 1) % n;

            result[vertex_index++] = r + 1;
            neighbours.insert(r);
        }

        // sort the neighbours
        std::sort(result.begin() + (vertex_index - degrees[v]), result.begin() + vertex_index);
    }

    return result;
}

/**
 * A function that generates and returns a graph in the sorted representation.
 * It generates a graph with the exact number of vertices and some edges.
 *
 * @param n The number of vertices.
 * @param forbidden_degrees Nodes with the degrees from the vector will not be generated.
 * @param loops Whether loops are allowed.
 */
std::vector<int>
generate_graph(int n, const std::set<int> &forbidden_degrees = std::set<int>(), bool loops = false) {
    // set all degrees to be random
    int m = 0;
    std::vector<int> degrees(n);
    for (int &degree : degrees) {
        degree = random(0, n - 1);
        while (forbidden_degrees.contains(degree)) degree = (degree + 1) % n;
        m += degree;
    }

    std::vector<int> result(n + m + 2, 0);
    result[0] = n;
    result[n + 1] = m;

    int vertex_index = n + 2;
    for (int v = 0; v < n; v++) {
        result[v + 1] = vertex_index;

        // generate random neighbours
        // we're not working on multi-graphs, so use set to prevent adding repeated neighbours
        std::set<int> neighbours;

        // possibly prevent loops
        if (!loops)
            neighbours.insert(v);

        for (int i = 0; i < degrees[v]; i++) {
            int r = random(0, n);
            while (neighbours.contains(r))
                r = (r + 1) % n;

            result[vertex_index++] = r + 1;
            neighbours.insert(r);
        }

        // sort the neighbours
        std::sort(result.begin() + (vertex_index - degrees[v]), result.begin() + vertex_index);
    }

    return result;
}

/**
 * Attach a formatted graph representation to a debug message.
 */
std::string attach_graph(std::string m, const std::vector<int> &graph) {
    m = m + " [";
    int i = 0;
    for (; i < graph.size() - 1; ++i)
        m += std::to_string(graph[i]) + ", ";
    return m + std::to_string(graph[i]) + "]";
}

/**
 * Check the correctness of the graph, given in the sorted standard representation.
 */
void check_graph_correctness(std::vector<int> &graph, int n, int m,
                             const std::set<int> &forbidden_degrees = std::set<int>(), bool loops = false) {
    ASSERT_EQ(vertices(graph), n) << attach_graph(
                        "Number of vertices in the generated graph doesn't match the wanted number.", graph);

    ASSERT_EQ(edges(graph), m) << attach_graph(
                        "Number of edges in the generated graph doesn't match the wanted number.", graph);

    ASSERT_EQ(graph.size(), n + m + 2) << attach_graph("The graph has an incorrect size.", graph);

    // check that the graph doesn't contain forbidden degrees
    for (int v = 1; v <= n; v++)
        ASSERT_FALSE(forbidden_degrees.contains(neighbours(graph, v).size())) << attach_graph(
                            "Vertex " + std::to_string(v) + " has a forbidden degree " +
                            std::to_string(neighbours(graph, v).size()) + ":", graph);

    // check that the vertices of the graph are sorted
    for (int v = 1; v <= n - 1; v++)
        ASSERT_TRUE(graph[v] <= graph[v + 1])
                                    << attach_graph("Vertices " + std::to_string(v) + " has a forbidden degree:",
                                                    graph);

    // check that each neighbour value is in a valid range
    for (int v = n + 2; v < n + m + 2; v++)
        ASSERT_TRUE(1 <= graph[v] && graph[v] <= n) << attach_graph(
                            "Vertex " + std::to_string(v) + " has a neighbour outside vertex range:", graph);

    // check that the vertices point to reasonable places
    for (int v = 1; v <= n; v++)
        ASSERT_TRUE(n + 1 <= graph[v] && graph[v] <= n + m + 2)
                                    << attach_graph("Vertex " + std::to_string(v) + " points to an incorrect index:",
                                                    graph);

    // check that neighbours of each vertex are sorted
    // also check for loops
    for (int v = 1; v <= n; v++) {
        auto nb = neighbours(graph, v);

        // care! nb.size() - 1 is unsigned, so 0 - 1 is gigantic
        for (int i = 1; i < nb.size(); i++)
            ASSERT_TRUE(nb[i - 1] < nb[i]) << attach_graph(
                                "The neighbours " + std::to_string(nb[i - 1]) + " and " + std::to_string(nb[i]) +
                                " of vertex " + std::to_string(v) + " are not sorted:", graph);

        // check for loops
        if (!loops)
            for (int i : nb)
                ASSERT_TRUE(i != v) << attach_graph("The vertex " + std::to_string(v) + " contains a loop:", graph);
    }
}

/**
 * Check graphs of the given sizes.
 */
std::vector<int> generate_random_graph(int n_lo, int n_hi, const std::set<int> &forbidden_degrees = std::set<int>(),
                                       bool loops = false) {
    int n = random(n_lo, n_hi);

    // use different generation algorithms, depending on whether we want to forbid certain degrees
    if (forbidden_degrees.empty()) {
        // random(0, 0) has undefined behavior, because hi si not included
        // a complete directed graph has n(n - 1) edges.
        int m = (n == 1) ? 0 : random(0, n * (n - 1) + 1);

        return generate_graph(n, m, loops);
    } else {
        return generate_graph(n, forbidden_degrees, loops);
    }
}

/**
 * Check the correctness of the order.
 * Positive values in the vector signal a calls to preprocess, negative to postprocess.
 *
 * @param graph The graph to check DFS order on.
 * @param order A vector of values where +v means "entered a vertex" and -v means "left it". Indexed from 1.
 * @param start The starting vertex. Indexed from 1.
 */
void check_dfs_order(std::vector<int> &graph, std::vector<int> order, int start) {
    // find all accessible vertices from the start using BFS
    std::vector<bool> explored(vertices(graph));
    std::queue<int> queue;
    queue.push(start - 1);
    explored[start - 1] = true;
    while (!queue.empty()) {
        int current = queue.front();
        queue.pop();

        for (int neighbour: neighbours(graph, current + 1)) {
            if (!explored[neighbour - 1]) {
                explored[neighbour - 1] = true;
                queue.push(neighbour - 1);
            }
        }
    }

    // check that all of them have been explored
    for (int i : order) explored[abs(i) - 1] = false;
    for (int i = 0; i < explored.size(); ++i)
        ASSERT_TRUE(!explored[i]) << attach_graph(
                            "The vertex " + std::to_string(i) + " is accessible but was not explored.", graph);

    ASSERT_EQ(order[0], start) << attach_graph("The starting vector is not entered first.", graph);
    ASSERT_EQ(order[order.size() - 1], -start) << attach_graph("The starting vector is not exited last.", graph);

    // simulate the DF
    std::stack<int> path;
    path.push(start);

    for (int v : order) {
        if (abs(v) == start) continue;

        // if we're appending
        if (v > 0) {
            auto nb = neighbours(graph, path.top());

            // check if we can go to that vertex
            ASSERT_TRUE(std::find(nb.begin(), nb.end(), v) != nb.end()) << attach_graph(
                                "Vertex " + std::to_string(path.top()) + " does not contain vertex " +
                                std::to_string(v) + " as a neighbour.", graph);

            path.push(v);
        } else {
            ASSERT_EQ(path.top(), abs(v)) << attach_graph(
                                "Postprocess was called on vertex " + std::to_string(abs(v)) + ", although we're in " +
                                std::to_string(path.top()) + ":", graph);
            path.pop();
        }
    }

    // check that no vertex is either both entered and visited or neither
    // done by checking whether both preprocess and postprocess is called
    // done last, since it messes with the array
    std::sort(order.begin(), order.end(), [](int a, int b) { return abs(a) < abs(b); });
    for (int i = 0; i < order.size(); i += 2)
        ASSERT_EQ(order[i], -order[i + 1]) << attach_graph(
                            "Preprocess and postprocess not called on " + std::to_string(order[i]) + ".", graph);
}

/**
 * Test the correct generation of graphs with random parameters.
 */
void test_graph_generation(int n_lo, int n_hi, const std::set<int> &forbidden_degrees = std::set<int>(),
                           bool loops = false) {
    for (int i = 0; i < GENERATIONS; ++i) {
        auto graph = generate_random_graph(n_lo, n_hi, forbidden_degrees, loops);

        check_graph_correctness(graph, vertices(graph), edges(graph), forbidden_degrees, loops);
    }
}

/**
 * Test the correct generation of graphs with random parameters.
 */
void test_dfs_correctness(int n_lo, int n_hi) {
    for (int i = 0; i < GENERATIONS; ++i) {
        std::vector<int> order;
        auto graph = generate_random_graph(n_lo, n_hi);
        int start = random(0, vertices(graph));

        // a DFS run that stores the vertex order in a vector to check for correctness
        // note that they are indexed from 1, because -0 == 0...
        dfs_linear_memory(
                graph,
                start,
                [&order](int v) { order.push_back(v + 1); },
                [&order](int v) { order.push_back(-v - 1); }
        );

        check_dfs_order(graph, order, start + 1);
    }
}

/**
 * Test, whether converting between the representations (when they're not modified) perserves the graph.
 */
void test_representations_correctness(int n_lo, int n_hi) {
    for (int i = 0; i < GENERATIONS; ++i) {
        auto graph = generate_random_graph(n_lo, n_hi);
        auto graph_sorted(graph);

        sorted_to_pointer(graph);
        pointer_to_sorted(graph);
        ASSERT_EQ(graph_sorted, graph) << "sorted -> pointer -> sorted conversion produced a different graph.";

        sorted_to_pointer(graph);
        auto graph_pointer(graph);

        pointer_to_swap(graph);
        swap_to_pointer(graph);
        ASSERT_EQ(graph_pointer, graph) << "pointer -> swap -> pointer conversion produced a different graph.";

        pointer_to_sorted(graph);
        ASSERT_EQ(graph_sorted, graph) << "sorted -> pointer -> sorted conversion produced a different graph.";
    }
}

// TODO: merge tests to not repeatedly generate graphs (generate -> test graph -> test regular DFS -> test conversions -> tests constant DFS)

//@formatter:off

TEST(ArrayTestSuite, TestSmall) { test_graph_generation(SMALL); }
TEST(ArrayTestSuite, TestMedium) { test_graph_generation(MEDIUM); }
#if LARGE_TESTS
TEST(ArrayTestSuite, TestLarge){ test_graph_generation(LARGE); }
#endif

TEST(ArrayTestSuite, GenerateSmallNoZeroDegrees) { test_graph_generation(SMALL, std::set{0}); }
TEST(ArrayTestSuite, GenerateMediumNoZeroDegrees) { test_graph_generation(MEDIUM, std::set{0}); }
#if LARGE_TESTS
TEST(ArrayTestSuite, GenerateLargeNoZeroDegrees){ test_graph_generation(LARGE, std::set{0}); }
#endif

TEST(ArrayTestSuite, GenerateSmallNoOneDegrees) { test_graph_generation(SMALL, std::set{1}); }
TEST(ArrayTestSuite, GenerateMediumNoOneDegrees) { test_graph_generation(MEDIUM, std::set{1}); }
#if LARGE_TESTS
TEST(ArrayTestSuite, GenerateLargeNoOneDegrees){ test_graph_generation(LARGE, std::set{1}); }
#endif

TEST(ArrayTestSuite, GenerateSmallNoZeroOneDegrees) { test_graph_generation(SMALL, std::set{0, 1}); }
TEST(ArrayTestSuite, GenerateMediumNoZeroOneDegrees) { test_graph_generation(MEDIUM, std::set{0, 1}); }
#if LARGE_TESTS
TEST(ArrayTestSuite, GenerateLargeNoZeroOneDegrees){ test_graph_generation(LARGE, std::set{0, 1}); }
#endif

TEST(ArrayTestSuite, DFSLinearSmall) { test_dfs_correctness(SMALL); }
TEST(ArrayTestSuite, DFSLinearMedium) { test_dfs_correctness(MEDIUM); }
#if LARGE_TESTS
TEST(ArrayTestSuite, DFSLinearLarge){ test_dfs_correctness(LARGE); }
#endif

TEST(ArrayTestSuite, TestRepresentationsSmall) { test_representations_correctness(SMALL); }
TEST(ArrayTestSuite, TestRepresentationsMedium) { test_representations_correctness(MEDIUM); }
#if LARGE_TESTS
TEST(ArrayTestSuite, TestRepresentationsLarge){ test_representations_correctness(LARGE); }
#endif

// TODO: tests including the forbidden edges
// TODO: constant memory DFS tests

//@formatter:on
