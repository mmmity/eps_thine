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


    // first-improve
    ProgressVisualizer vis(1000);
    for (int i = 0; i * points.size() * points.size() * points.size() < 1000000000; ++i) {
      vis.add_progress(1);
      double current_result = result;
      bool changed = false;
      for (int i = 0; i + 2 < points.size(); ++i) {
        for (int j = i + 2; j < points.size(); ++j) {
          two_opt(i, j);
          if (result < current_result + 1e-15) {
            changed = true;
            current_result = result;
            break;
          }
          two_opt(i, j);
        }
        if (changed) break;
      }
      if (!changed) break;
    }
}