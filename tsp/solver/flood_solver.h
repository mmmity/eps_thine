#pragma once

#include <random>
#include "ls_solver.h"

class FloodTSPSolver : public TSPSolver {
 public: 
  FloodTSPSolver(std::vector<point>&& points) : TSPSolver(std::move(points)) {}
  FloodTSPSolver(const std::vector<point>& points) : TSPSolver(points) {}

  void solve() override;

 private:
  int candidate_count = 100;
  double increase = 20;
  int times_for_2opt = 10;

  void choose_constants();
  std::vector<LSTSPSolver> candidates;
};