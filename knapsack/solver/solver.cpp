#include "solver.h"

bool KSSolver::check() {
  long long mass = 0;
  long long cost = 0;
  for (int i : result_set) {
    mass += weights[i];
    cost += costs[i];
  }

  return mass <= W && cost == result;
}