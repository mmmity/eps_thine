#include "greedy_solver.h"

#include <numeric>
#include <algorithm>
#include <iostream>

void GreedyKSSolver::solve() {
  std::vector<int> sorted_indices(costs.size());
  std::iota(sorted_indices.begin(), sorted_indices.end(), 0);

  std::sort(sorted_indices.begin(), sorted_indices.end(),
            [this](int i, int j) {
              return costs[i] * weights[j] > costs[j] * weights[i];
            });
  
  long long current_weight = 0;
  long long current_cost = 0;
  std::vector<int> top_indices;
  std::vector<bool> used(sorted_indices.size(), false);

  for (int i : sorted_indices) {
    if (current_weight + weights[i] <= W) {
      current_weight += weights[i];
      top_indices.push_back(i);
      current_cost += costs[i];
      used[i] = true;
    }
  }

  // now we find first viable unused to compare it with current result
  int first_unused = -1;
  for (int i : sorted_indices) {
    if (!used[i] && weights[i] <= W) {
      first_unused = i;
      break;
    }
  }

  if (first_unused != -1 && current_cost < costs[first_unused]) {
    result = costs[first_unused];
    result_set = {first_unused};
    return;
  }

  result = current_cost;
  result_set = std::move(top_indices);
}