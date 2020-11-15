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
