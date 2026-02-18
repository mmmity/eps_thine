#include "solver.h"

class GreedySSCSolver : public SSCSolver {
 public: 
  GreedySSCSolver(int n, std::vector<std::vector<int>>&& sets, std::vector<long long>&& costs) : SSCSolver(n, std::move(sets), std::move(costs)) {}
  GreedySSCSolver(int n, const std::vector<std::vector<int>>& sets, const std::vector<long long>& costs) : SSCSolver(n, sets, costs) {}

  void solve() override;

  ~GreedySSCSolver() {}
};