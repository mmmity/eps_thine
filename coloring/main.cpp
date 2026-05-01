#include <ld_solver.h>
#include <fstream>
#include <iostream>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "usage: ./coloring <test_filename> <result_filename>";
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

  LDColSolver solver(std::move(graph));

  solver.solve();

  // if (!solver.check()) {
  //   std::cout << "SOLVER DOES NOT WORK AS INTENDED\n";
  //   return 1;
  // }

  std::ofstream fout(argv[2]);
  fout << solver.get_result() << "\n";
  for (int i : solver.get_result_col()) {
    fout << i << " ";
  }
  fout << "\n";

  std::cout << solver.get_result() << "\n";
}