#include "solver.h"
#include <random>

class GreedyColSolver : public ColSolver {
 public:
  GreedyColSolver(std::vector<std::vector<int>>&& graph) : ColSolver(std::move(graph)) {}
  GreedyColSolver(const std::vector<std::vector<int>>& graph) : ColSolver(graph) {}

  void solve() override;

  ~GreedyColSolver() {};
 
 private:
  void try_make_greedy_solution(std::mt19937& rnd);
};