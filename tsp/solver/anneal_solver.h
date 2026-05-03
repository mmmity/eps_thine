#pragma once

#include <random>
#include "ls_solver.h"

class AnnealTSPSolver : public GreedyTSPSolver {
 public: 
  AnnealTSPSolver(std::vector<point>&& points) : GreedyTSPSolver(std::move(points)) {}
  AnnealTSPSolver(const std::vector<point>& points) : GreedyTSPSolver(points) {}

  void solve() override;

  ~AnnealTSPSolver() {}

 private:
  int restart_count = 500;
  int opt_count = 500000;
  double start_temp = 1000;
  double end_temp = 1e-4;
  int nn_count = 30;
  double temp, mul;
  std::vector<std::vector<int>> nearest_neighbors;
  std::vector<int> point_to_pos;

  void calc_nearest_neighbors();

  void solve_once(std::mt19937& rnd);
  void choose_constants();
  void random_2opt(std::mt19937& rnd);

  void two_opt(int i, int j);
  double mock_two_opt(int i, int j);
};