#pragma once

#include "greedy_solver.h"
#include <random>

class LSTSPSolver : public GreedyTSPSolver {
 public: 

  LSTSPSolver(std::vector<point>&& points) : GreedyTSPSolver(std::move(points)) {}
  LSTSPSolver(const std::vector<point>& points) : GreedyTSPSolver(points) {}

  void solve() override;

  ~LSTSPSolver() {}

 private:
  void two_opt(int i, int j);
  double mock_two_opt(int i, int j);
};