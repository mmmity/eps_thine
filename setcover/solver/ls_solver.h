#pragma once

#include "solver.h"
#include <vis.h>

#include <random>
#include <iostream>

class LSSSCSolver : public SSCSolver {
 public:
  LSSSCSolver(int n,
               std::vector<std::vector<int>>&& sets,
               std::vector<long long>&& costs) : 
               SSCSolver(n, std::move(sets), std::move(costs)) {
    calc_covering();
  }
  LSSSCSolver(int n,
               const std::vector<std::vector<int>>& sets,
               const std::vector<long long>& costs) : 
               SSCSolver(n, sets, costs) {
    calc_covering();
  }

  void solve() override;

  ~LSSSCSolver() override {}

 private:
  std::mt19937 rnd{1000-7};
  const double EPS = 1e-8;
  const int grasp_top_k = 30;
  const int MAX_STEPS = 15000;
  
  std::vector<std::vector<int>> covering;
  std::vector<double> element_weights;

  size_t steps{0};
  ProgressVisualizer vis{MAX_STEPS};
  
  void calc_covering();
  long long calc_cost(const std::vector<int>& solution);
  void update_best(const std::vector<int>& solution);

  void fix_solution(std::vector<int>& solution);
  void remove_redundant(std::vector<int>& solution);

  std::vector<int> grasp(int top_k);

  size_t current_result{0};
};