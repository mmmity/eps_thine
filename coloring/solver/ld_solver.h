#pragma once

#include "solver.h"

#include <random>

// локальный спуск - покрасив жадно, будем случайно переставлять классы и упорядочивать вершины по классам
// (LD - local descent)

class LDColSolver : public ColSolver {
 public:
  LDColSolver(std::vector<std::vector<int>>&& graph) : ColSolver(std::move(graph)) {}
  LDColSolver(const std::vector<std::vector<int>>& graph) : ColSolver(graph) {}

  void solve() override;

  ~LDColSolver() override {};
 
 private:
  int make_greedy_solution(const std::vector<int>& order, std::vector<int>& output_solution);
  void try_make_solution(std::mt19937& rnd);
  int optimize_current_solution(std::mt19937& rnd, std::vector<int>& current_solution);

  std::vector<int> best_result;

  const int seed = 1000-7;
  const int outer_max_steps = 100;
  const int inner_max_steps = 300;
};