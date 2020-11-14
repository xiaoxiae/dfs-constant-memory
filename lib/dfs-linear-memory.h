#pragma once

#include <vector>

template<typename Pre, typename Post>
void dfs_linear_memory(std::vector<int> &graph, int start, Pre& preprocess, Post& postprocess);
