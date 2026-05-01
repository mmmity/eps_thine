#pragma once

#include <vector>
#include <cmath>

struct point {
    double x, y;
};

class TSPSolver {
 protected:
  std::vector<point> points;

  std::vector<std::vector<double>> distance_matrix;

  std::vector<int> result_set;
  double result{-1};

  const double EPS = 1e-6;

 public:

  static double distance(point a, point b) {
    return std::hypot(a.x - b.x, a.y - b.y);
  }

  TSPSolver(std::vector<point>&& points) : points(std::move(points)) {
    distance_matrix.resize(points.size(), std::vector<double>(points.size()));
    for (int i = 0; i < points.size(); ++i) {
      for (int j = 0; j < points.size(); ++j) {
        distance_matrix[i][j] = distance(points[i], points[j]);
      }
    }
  }
  TSPSolver(const std::vector<point>& points) : points(points) {
    distance_matrix.resize(points.size(), std::vector<double>(points.size()));
    for (int i = 0; i < points.size(); ++i) {
      for (int j = 0; j < points.size(); ++j) {
        distance_matrix[i][j] = distance(points[i], points[j]);
      }
    }
  }

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