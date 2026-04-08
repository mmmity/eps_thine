#pragma once

#include "solver.h"
#include <vis.h>

#include <optional>
#include <random>
#include <iostream>

struct dp_result {
    std::vector<long long> dist;
    std::vector<int> parent;
    std::vector<int> parent_set;
};

class RandomizedElementSSCSolver : public SSCSolver {
 public:
  RandomizedElementSSCSolver(int n,
               std::vector<std::vector<int>>&& sets,
               std::vector<long long>&& costs,
               size_t seed) : 
               SSCSolver(n, std::move(sets), std::move(costs)), rnd(seed) {
    covering.resize(n);
    for (int i = 0; i < this->sets.size(); ++i) {
      for (int j : this->sets[i]) covering[j].push_back(i);
    }
    coverage_matrix.resize(sets.size());
    for (int i = 0; i < sets.size(); ++i) {
      coverage_matrix[i].resize(n, false);
      for (int j : sets[i]) coverage_matrix[i][j] = true;
    }
  }
  RandomizedElementSSCSolver(int n,
               const std::vector<std::vector<int>>& sets,
               const std::vector<long long>& costs,
               size_t seed) : 
               SSCSolver(n, sets, costs), rnd(seed) {
    covering.resize(n);
    for (int i = 0; i < sets.size(); ++i) {
      for (int j : sets[i]) covering[j].push_back(i);
    }
    coverage_matrix.resize(sets.size());
    for (int i = 0; i < sets.size(); ++i) {
      coverage_matrix[i].resize(n, false);
      for (int j : sets[i]) coverage_matrix[i][j] = true;
    }
  }

  void solve() override;

  ~RandomizedElementSSCSolver() override {}

 private:
  const int MAX_STEPS = 500;
  const int SAMPLE_SIZE = 21;
  ProgressVisualizer vis{MAX_STEPS};
  long long current_result = 0;
  long long one_sample_result = 0;
  std::vector<int> one_sample_result_set;
  std::vector<int> current_result_set;

  std::vector<std::vector<int>> covering;
  std::mt19937 rnd;

  void solve_once();
  void solve_n_times(int n);
  void solve_for_sample(const std::vector<int>& sample);
  void greedy_finish();
  void apply_fine();
  std::vector<int> fine;
  std::vector<std::vector<bool>> coverage_matrix;
};