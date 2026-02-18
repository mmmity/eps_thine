#include "greedy_solver.h"

void GreedySSCSolver::solve() {
  std::vector<bool> covered(n, false);
  std::vector<bool> used(sets.size(), false);

  int yet_to_cover = n;
  int unused = sets.size();
  result = 0;
  
  while (yet_to_cover > 0 && unused > 0) {
    int best_set_idx = -1;
    int best_count = -1;
    long long best_cost = -1;
    for (int i = 0; i < sets.size(); ++i) {
      if (used[i]) continue;

      int newly_covered = 0;
      for (int j : sets[i]) {
        if (!covered[j]) ++newly_covered;
      }

      if (best_set_idx == -1 || newly_covered * best_cost > best_count * costs[i]) {
        best_set_idx = i;
        best_count = newly_covered;
        best_cost = costs[i];
      }      
    }

    for (int i : sets[best_set_idx]) {
      covered[i] = true;
    }
    yet_to_cover -= best_count;
    result += best_cost;
  }
}