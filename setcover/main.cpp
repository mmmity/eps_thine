#include <iostream>
#include <fstream>
#include <solver/greedy_solver.h>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "usage: ./setcover <test_filename>";
    return 1;
  }

  std::ifstream fin(argv[1]);
  int n, m;
  fin >> n >> m;

  std::vector<long long> costs(m);
  std::vector<std::vector<int>> sets(m);

  for (int i = 0; i < m; ++i) {
    fin >> costs[i];

    int el;
    while (fin.peek() != '\n' && fin >> el) {
      sets[i].push_back(el);
    }
  }

  GreedySSCSolver solver(n, std::move(sets), std::move(costs));

  solver.solve();

  if (!solver.check()) {
    std::cout << "SOLVER DOES NOT WORK AS INTENDED\n";
    return 1;
  }

  std::cout << solver.get_result();
}