#pragma once

#include "solver.h"

#include <random>
#include <iostream>

class RandomizedSSCSolver : public SSCSolver {
 public:
  RandomizedSSCSolver(int n,
               std::vector<std::vector<int>>&& sets,
               std::vector<long long>&& costs,
               size_t seed) : 
               SSCSolver(n, std::move(sets), std::move(costs)), rnd(seed) {
    covering.resize(n);
    for (int i = 0; i < this->sets.size(); ++i) {
      for (int j : this->sets[i]) covering[j].push_back(i);
    }
  }
  RandomizedSSCSolver(int n,
               const std::vector<std::vector<int>>& sets,
               const std::vector<long long>& costs,
               size_t seed) : 
               SSCSolver(n, sets, costs), rnd(seed) {
    covering.resize(n);
    for (int i = 0; i < sets.size(); ++i) {
      for (int j : sets[i]) covering[j].push_back(i);
    }
  }

  void solve() override;

  ~RandomizedSSCSolver() {}

 private:  
  std::vector<std::vector<int>> covering;
  std::vector<double> lb_weights;
  std::vector<size_t> indices;
  std::mt19937 rnd;

  double lower_bound();
  void solve_once();
  void remove_covered();
  void greedy_finish();
};