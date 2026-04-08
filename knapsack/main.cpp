#pragma comment(linker, "/STACK:16777216")

#include <iostream>
#include <fstream>
#include <bnb_solver.h>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "usage: ./knapsack <test_filename> <result_filename>";
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

  BnBKSSolver solver(W, std::move(weights), std::move(costs));

  solver.solve();

  if (!solver.check()) {
    std::cout << "SOLVER DOES NOT WORK AS INTENDED\n";
    return 1;
  }

  std::ofstream fout(argv[2]);
  fout << solver.get_result_set().size() << " " << solver.get_result() << "\n";
  for (int i : solver.get_result_set()) {
    fout << i << " ";
  }
  fout << "\n";

  std::cout << solver.get_result() << "\n";
}