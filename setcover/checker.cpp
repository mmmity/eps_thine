#include <iostream>
#include <fstream>
#include <vector>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "usage: ./solver <prefix> <testname>\n";
    return 1;
  }

  std::string prefix(argv[1]);
  std::string testname(argv[2]);

  std::ifstream inp(prefix + "/tests/" + testname);
  std::ifstream out(prefix + "/results/" + testname);

  int n, m;
  inp >> n >> m;

  std::vector<long long> costs(m);
  std::vector<std::vector<int>> sets(m);

  for (int i = 0; i < m; ++i) {
    inp >> costs[i];

    int el;
    while (inp.peek() != '\n' && inp >> el) {
      sets[i].push_back(el);
    }
  }

  int p;
  long long result;
  out >> p >> result;
  std::vector<int> indices(p);
  for (int i = 0; i < p; ++i) {
    out >> indices[i];
  }

  std::vector<bool> covered(n, false);
  for (int i : indices) {
    for (int j : sets[i]) {
      covered[j] = true;
    }
  }
  for (int j = 0; j < n; ++j) {
    if (!covered[j]) {
      std::cerr << "Failed test " << testname << ": element " << j << " not covered\n";
      return 1;
    }
  }

  long long real_result = 0;
  for (int i : indices) {
    real_result += costs[i];
  }

  if (result != real_result) {
    std::cerr << "Failed test " << testname << ": result " << result << " != " << real_result << "\n";
    return 1;
  }
  
  return 0;
}