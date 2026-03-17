#include <greedy_solver.h>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  if (argc < 2) {
    std::cerr << "usage: ./coloring <test_filename>";
    return 1;
  }

  std::ifstream fin(argv[1]);
  int n, m;
  fin >> n >> m;
  std::vector<std::vector<int>> graph(n);

  for (int i = 0; i < m; ++i) {
    int u, v;
    fin >> u >> v;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }

  GreedyColSolver solver(std::move(graph));

  solver.solve();

  if (!solver.check()) {
    std::cout << "SOLVER DOES NOT WORK AS INTENDED\n";
    return 1;
  }

  std::cout << solver.get_result() << "\n";
}