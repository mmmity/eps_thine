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

  int N, W;
  inp >> N >> W;
  std::vector<int> costs(N), weights(N);
  for (int i = 0; i < N; ++i) {
    inp >> costs[i] >> weights[i];
  }

  int n, c;
  out >> n >> c;
  std::vector<int> idx(n);
  for (int i = 0; i < n; ++i) out >> idx[i];

  std::vector<bool> used(N);
  for (int i : idx) {
    if (used[i]) {
      std::cerr << "Failed test " << testname << ": item " << i << " used twice\n";
      return 1;
    }
    used[i] = true; 
  }

  int real_w = 0;
  for (int i = 0; i < n; ++i) {
    real_w += weights[idx[i]];
  }

  int real_c = 0;
  for (int i = 0; i < n; ++i) {
    real_c += costs[idx[i]];
  }

  if (real_w > W) {
    std::cerr << "Failed test " << testname << ": weight limit exceeded\n";
    return 1;
  }

  if (real_c != c) {
    std::cerr << "Failed test " << testname << ": costs do not align\n";
    return 1;
  }

  return 0;
}