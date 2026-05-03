#include <iomanip>
#include <iostream>
#include <fstream>
#include <anneal_solver.h>
#include <ls_solver.h>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "usage: ./tsp <test_filename> <result_filename>";
    return 1;
  }

  std::ifstream fin(argv[1]);
  int n;
  fin >> n;

  std::vector<point> pts(n);

  for (int i = 0; i < n; ++i) {
    fin >> pts[i].x >> pts[i].y;
  }

  AnnealTSPSolver solver(std::move(pts));

  solver.solve();

  if (!solver.check()) {
    std::cout << "SOLVER DOES NOT WORK AS INTENDED\n";
    return 1;
  }

  std::ofstream fout(argv[2]);
  fout << std::setprecision(10) << std::fixed << solver.get_result() << "\n";
  for (int i : solver.get_result_set()) {
    fout << i << " ";
  }
  fout << "\n";

  std::cout << std::setprecision(10) << std::fixed << solver.get_result() << "\n";
}