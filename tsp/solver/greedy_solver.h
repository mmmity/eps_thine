#pragma once

#include "solver.h"

class GreedyTSPSolver : public TSPSolver {
 public: 

  GreedyTSPSolver(std::vector<point>&& points) : TSPSolver(std::move(points)) {}
  GreedyTSPSolver(const std::vector<point>& points) : TSPSolver(points) {}

  void solve() override;

  ~GreedyTSPSolver() {}
};