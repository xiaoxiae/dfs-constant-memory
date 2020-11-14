#pragma once

#include <vector>
#include <span>

int vertices(std::vector<int> &graph);

int edges(std::vector<int> &graph);

std::span<int> neighbours(std::vector<int> &graph, int vertex);

void sorted_to_pointer(std::vector<int> &graph);

void pointer_to_sorted(std::vector<int> &graph);

void pointer_to_swap(std::vector<int> &graph);

void swap_to_pointer(std::vector<int> &graph);

void swap_to_sorted(std::vector<int> &graph);

