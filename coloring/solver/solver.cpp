#include "solver.h"

bool ColSolver::check() {
  for (int i = 0; i < graph.size(); ++i) {
    for (int v : graph[i]) {
      if (result_col[i] == result_col[v]) {
        return false;
      }
    }
  }
  return true;
}