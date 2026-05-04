#pragma once

#include <vector>
#include <cmath>

struct point {
    double x, y;
};

class FacilitySolver {
 protected:
  std::vector<point> facilities;
  std::vector<double> costs;
  std::vector<double> caps;
  std::vector<point> consumers;
  std::vector<double> demands;

  std::vector<int> assigned_facilities;
  double result{0};

  const double EPS = 1e-5;

 public:

  static double distance(point a, point b) {
    return std::hypot(a.x - b.x, a.y - b.y);
  }

  FacilitySolver(const std::vector<point>& facilities,
                 const std::vector<double> costs,
                 const std::vector<double> caps,
                 const std::vector<point>& consumers,
                 const std::vector<double>& demands
                ) : facilities(facilities), costs(costs), caps(caps), consumers(consumers), demands(demands) {
                    assigned_facilities.resize(consumers.size(), -1);
                }

  virtual void solve() {};

  double get_result() {
    return result;
  }

  const std::vector<int>& get_assigned_facilities() {
    return assigned_facilities;
  }

  bool check();

  ~FacilitySolver() {}
};