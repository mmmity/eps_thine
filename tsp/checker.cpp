#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

struct point {
    double x, y;
};

double dist(point a, point b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

int main(int argc, char** argv) {
  if (argc < 3) {
    std::cerr << "usage: ./solver <prefix> <testname>\n";
    return 1;
  }

  std::string prefix(argv[1]);
  std::string testname(argv[2]);

  std::ifstream inp(prefix + "/tests/" + testname);
  std::ifstream out(prefix + "/results/" + testname);
  int n;
  inp >> n;
  std::vector<point> pts(n);
  for (int i = 0; i < n; ++i) {
    inp >> pts[i].x >> pts[i].y;
  }

  double given_dist;
  std::vector<int> order(n);
  out >> given_dist;
  for (int i = 0; i < n; ++i) out >> order[i];

  std::vector<bool> used(n, false);

  for (int i : order) {
    if (used[i]) {
      std::cerr << "Failed test " << testname << ": point " << i << " visited twice\n";
      return 1;
    }
    used[i] = true; 
  }

  for (int i : order) {
    if (!used[i]) {
        std::cerr << "Failed test " << testname << ": point " << i << " not visited\n";
        return 1;
    }
  }

  double actual_dist = 0;
  for (int i = 1; i < n; ++i) {
    actual_dist += dist(pts[order[i - 1]], pts[order[i]]);
  }
  actual_dist += dist(pts[order[n - 1]], pts[order[0]]);

  if (std::abs(actual_dist - given_dist) > 1e-4) {
    std::cerr << "Failed test " << testname << ": results not equal\n";
    std::cerr << actual_dist << " " << given_dist << "\n";
    return 1;
  }

  return 0;
}