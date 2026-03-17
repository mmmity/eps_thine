#include <vector>
#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cout << "usage: ./checker <prefix> <testname>";
    return 1;
  }

  std::string prefix(argv[1]);
  std::string testname(argv[2]);

  std::ifstream inp(prefix + "/tests/" + testname);
  std::ifstream out(prefix + "/results/" + testname);

  int n, m;
  inp >> n >> m;
  std::vector<std::vector<int>> graph(n);
  for (int i = 0; i < m; ++i) {
    int u, v;
    inp >> u >> v;
    graph[u].push_back(v);
    graph[v].push_back(u);
  }

  int res;
  out >> res;
  std::vector<int> col(n);
  for (int i = 0; i < n; ++i) {
    if (out.eof()) {
      std::cerr << "Failed test " << testname << ": not enough values in output" << std::endl;
      return 1;
    }

    out >> col[i];
  }

  for (int i = 0; i < n; ++i) {
    for (int j : graph[i]) {
      if (col[i] >= res) {
        std::cerr << "Failed test " << testname << ": color is too big" << std::endl;
        return 1;
      }
      if (col[i] == col[j]) {
        std::cerr << "Failed test " << testname << ": colors of " << i << " and " << j << " coincide" << std::endl;
        return 1;
      }
    }
  }

  return 0;
}