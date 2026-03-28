#pragma once

#include <vector>
#include <cmath>

struct point {
    double x, y;
};

class TSPSolver {
 protected:
  std::vector<point> points;

  std::vector<int> result_set;
  double result{0};

  const double EPS = 1e-6;

 public:

  static double distance(point a, point b) {
    return std::hypot(a.x - b.x, a.y - b.y);
  }

  TSPSolver(std::vector<point>&& points) : points(std::move(points)) {}
  TSPSolver(const std::vector<point>& points) : points(points) {}

  virtual void solve() {};

  double get_result() {
    return result;
  }

  const std::vector<int>& get_result_set() {
    return result_set;
  }

  bool check();

  ~TSPSolver() {}
};