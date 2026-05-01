#include "ls_solver.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <vis.h>

double LSTSPSolver::mock_two_opt(int i, int j) {
  double mock_result = result;
  mock_result -= distance_matrix[result_set[i]][result_set[(i + 1) % points.size()]];
  mock_result -= distance_matrix[result_set[j]][result_set[(j + 1) % points.size()]];
  mock_result += distance_matrix[result_set[i]][result_set[j]];
  mock_result += distance_matrix[result_set[(i + 1) % points.size()]][result_set[(j + 1) % points.size()]];
  return mock_result;
}

void LSTSPSolver::two_opt(int i, int j) {
  result -= distance_matrix[result_set[i]][result_set[(i + 1) % points.size()]];
  result -= distance_matrix[result_set[j]][result_set[(j + 1) % points.size()]];
  std::reverse(result_set.begin() + i + 1, result_set.begin() + j + 1);
  result += distance_matrix[result_set[i]][result_set[(i + 1) % points.size()]];
  result += distance_matrix[result_set[j]][result_set[(j + 1) % points.size()]];
}

void LSTSPSolver::solve() {
    GreedyTSPSolver::solve();

    // best-improve
    ProgressVisualizer vis(1000);
    for (int s = 0; s * points.size() * points.size() * points.size() < 1000000000; ++s) {
      vis.add_progress(1);
      double current_result = result;
      int best_i = -1, best_j = -1;
      for (int i = 0; i + 2 < points.size(); ++i) {
        for (int j = i + 2; j < points.size(); ++j) {
          double mock_result = mock_two_opt(i, j);
          if (mock_result < current_result) {
            current_result = mock_result;
            best_i = i;
            best_j = j;
          }
        }
      }
      if (best_i == -1) break;
      two_opt(best_i, best_j);
    }
}