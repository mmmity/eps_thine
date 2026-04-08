#pragma once

#include "solver.h"
#include <vis.h>

#include <random>
#include <iostream>

class BnBKSSolver : public KSSolver {
 public:
  struct EndOfBnB{};
  BnBKSSolver(int W, std::vector<long long>&& weights, std::vector<long long>&& costs) : KSSolver(W, std::move(weights), std::move(costs)) {
    current_state.resize(this->weights.size(), false);
    current_sure_state.resize(this->weights.size(), false);
  }
  BnBKSSolver(int W, const std::vector<long long>& weights, const std::vector<long long>& costs) : KSSolver(W, weights, costs) {
    current_state.resize(weights.size(), false);
    current_sure_state.resize(weights.size(), false);
  }

  void solve() override;

  ~BnBKSSolver() override {}

 private:
  const int MAX_STEPS = 300000;
  
  std::vector<std::vector<int>> covering;
  std::vector<int> element_freq;

  size_t steps{0};
  ProgressVisualizer vis{MAX_STEPS};
  std::vector<bool> current_state;
  std::vector<bool> current_sure_state;
  size_t current_sure_cost{0};
  size_t current_sure_weight{0};
  size_t current_cost{0};
  size_t current_weight{0};
  size_t current_state_end{0};
  void solve_recursive(int i);
  bool greedy_finish(int i);

  long long residue_lower_bound(size_t i);
  double residue_upper_bound(size_t i);

  size_t current_result{0};
};