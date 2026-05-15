#include <iostream>
#include <fstream>
#include <randomized_element_solver.h>
#include <ls_solver.h>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "usage: ./setcover <test_filename> <result_filename>";
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
    while (fin.peek() != '\n' && fin.peek() != '\r' && fin >> el) {
      sets[i].push_back(el);
    }
  }

#ifndef NDEBUG
  std::cout << n << " " << m << "\n";
  for (int i = 0; i < m; ++i) {
    std::cout << costs[i] << " ";
    for (int j : sets[i]) std::cout << j << " ";
    std::cout << "\n";
  }
#endif

  SSCSolver* solver{nullptr};

  if (n < 50) {
    solver = new RandomizedElementSSCSolver(n, std::move(sets), std::move(costs), 1000-7);
  } else {
    solver = new LSSSCSolver(n, std::move(sets), std::move(costs));
  }
  
  solver->solve();

  if (!solver->check()) {
    std::cout << "SOLVER DOES NOT WORK AS INTENDED\n";
    return 1;
  }

  std::ofstream fout(argv[2]);
  fout << solver->get_result_set().size() << " " << solver->get_result() << "\n";
  for (int i : solver->get_result_set()) {
    fout << i << " ";
  }
  fout << "\n";

  std::cout << solver->get_result() << "\n";

  delete solver;
}