#pragma once

#include "solver.h"
#include <vis.h>

#include <random>
#include <iostream>

struct EndOfBnB{};

class BnBSSCSolver : public SSCSolver {
 public:
  BnBSSCSolver(int n,
               std::vector<std::vector<int>>&& sets,
               std::vector<long long>&& costs) : 
               SSCSolver(n, std::move(sets), std::move(costs)) {
    current_state.resize(this->sets.size(), false);
    current_sure_state.resize(this->sets.size(), false);
    covered.resize(n, 0);
  }
  BnBSSCSolver(int n,
               const std::vector<std::vector<int>>& sets,
               const std::vector<long long>& costs) : 
               SSCSolver(n, sets, costs) {
    current_state.resize(sets.size(), false);
    current_sure_state.resize(sets.size(), false);
    covered.resize(n, 0);
  }

  void solve() override;

  ~BnBSSCSolver() {}

 private:
  const int MAX_STEPS = 15000;
  
  std::vector<std::vector<int>> covering;
  std::vector<int> element_freq;

  size_t steps{0};
  ProgressVisualizer vis{MAX_STEPS};
  std::vector<bool> current_state;
  std::vector<bool> current_sure_state;
  std::vector<int> covered;
  std::vector<int> indices;
  size_t current_sure_cost{0};
  size_t current_state_end{0};
  size_t current_cost{0};
  void solve_recursive(int i);
  bool greedy_finish(int i);
  bool check_useless(int i);

  double residue_lower_bound(size_t i);
  double residue_upper_bound(size_t i);

  size_t current_result{0};
};