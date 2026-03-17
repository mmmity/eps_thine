#include <iostream>
#include <fstream>
#include <greedy_solver.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "usage: ./knapsack <test_filename>";
    return 1;
  }

  std::ifstream fin(argv[1]);
  int n;
  long long W;
  fin >> n >> W;

  std::vector<long long> costs(n);
  std::vector<long long> weights(n);

  for (int i = 0; i < n; ++i) {
    fin >> costs[i] >> weights[i];
  }

  GreedyKSSolver solver(W, std::move(weights), std::move(costs));

  solver.solve();

  if (!solver.check()) {
    std::cout << "SOLVER DOES NOT WORK AS INTENDED\n";
    return 1;
  }

  std::cout << solver.get_result() << "\n";
}