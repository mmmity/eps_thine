#include "greedy_solver.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <mex.h>

void GreedyColSolver::try_make_greedy_solution(std::mt19937& rnd) {
  int n = graph.size();

  std::vector<int> vertex_order(n);
  std::iota(vertex_order.begin(), vertex_order.end(), 0);
  std::shuffle(vertex_order.begin(), vertex_order.end(), rnd);

  std::vector<int> coloring(n, -1);
  int max_col = 0;

  for (int i : vertex_order) {
    std::vector<int> neigh_colors;
    int current_max_color = 0;
    for (int u : graph[i]) {
      if (coloring[u] != -1) {
        current_max_color = std::max(current_max_color, coloring[u]);
        neigh_colors.push_back(coloring[u]);
      }
    }

    coloring[i] = mex(neigh_colors.begin(), neigh_colors.end(), current_max_color);
    max_col = std::max(max_col, coloring[i]);
  }

  if (result == -1 || result > max_col + 1) {
    result = max_col + 1;
    result_col = std::move(coloring);
  }
}

void GreedyColSolver::solve() {
  std::mt19937 rnd(1000-7);
  for (int i = 0; i < 500; ++i) {
    try_make_greedy_solution(rnd);
  }
}