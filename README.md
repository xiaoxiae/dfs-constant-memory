# DFS in constant memory [WIP, not finished]
A C++ implementation of the [Linear-Time In-Place DFS and BFS on the Word RAM](https://arxiv.org/abs/1803.04282v4) article, which allows to run a DFS on a graph (given a specific representation) in constant additional memory and linear time, as opposed to a standard DFS algorithms that use O(n) additional memory.

## Sample usage
```c++
#include "lib/dfs-constant-memory.h"
#include <vector>
#include <iostream>

void entering(int v)  { std::cout << "Entering vector " + std::to_string(v) + "." << std::endl;  }
void exiting(int v)  { std::cout << "Exiting vector " + std::to_string(v) + "." << std::endl;  }

int main() {
    std::vector<int> graph{5, 7, 9, 12, 14, 17, 12, 2, 5, 1, 3, 4, 2, 4, 2, 3, 5, 1, 4};
    int starting_vertex = 3;

    dfs_constant_memory(graph, starting_vertex, entering, exiting);
}
```
