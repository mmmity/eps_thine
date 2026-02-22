#include "solver.h"

bool SSCSolver::check() {
  std::vector<bool> covered(n, false);
  for (int i : result_set) {
    for (int j : sets[i]) {
      covered[j] = true;
    }
  }
  for (bool b : covered) {
    if (!b) return false;
  }
  return true;
}