#include "solver.h"

class GreedyKSSolver : public KSSolver {
 public: 

  GreedyKSSolver(int W, std::vector<long long>&& weights, std::vector<long long>&& costs) : KSSolver(W, std::move(weights), std::move(costs)) {}
  GreedyKSSolver(int W, const std::vector<long long>& weights, const std::vector<long long>& costs) : KSSolver(W, weights, costs) {}

  void solve() override;

  ~GreedyKSSolver() override {}
};