#include "solver.h"

#include <iostream>
#include <iomanip>

bool TSPSolver::check() {
  double apparent_result = 0;
  std::vector<bool> visited(points.size(), false);
  if (result_set.size() != points.size()) return false;
  visited[result_set[0]] = true;
  for (int i = 1; i < points.size(); ++i) {
    if (visited[result_set[i]]) {
      return false;
    }
    visited[result_set[i]] = true;
    apparent_result += TSPSolver::distance(points[result_set[i - 1]], points[result_set[i]]);
  }
  apparent_result += TSPSolver::distance(points[result_set[points.size() - 1]], points[result_set[0]]);
  for (int i = 0; i < points.size(); ++i) {
    if (!visited[i]) std::cout << i << "\n";
  }
  for (bool t : visited) {
    if (!t) return false;
  }
  return std::abs(apparent_result - result) < EPS;
}