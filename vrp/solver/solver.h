#pragma once

#include <vector>
#include <cmath>

struct VRPPoint {
    double x, y;
};

class VRPSolver {
 protected:
  int cars;
  double capacity;
  std::vector<VRPPoint> clients;
  std::vector<double> demands;

  std::vector<std::vector<int>> paths;
  double result{0};

  const double EPS = 1e-6;

 public:

  static double distance(VRPPoint a, VRPPoint b) {
    return std::hypot(a.x - b.x, a.y - b.y);
  }

  VRPSolver(const std::vector<VRPPoint>& clients,
            const std::vector<double>& demands,
            int cars,
            double capacity
            ) : clients(clients), demands(demands), cars(cars), capacity(capacity) {
                paths.resize(cars);
            }

  virtual void solve() {};

  double get_result() {
    return result;
  }

  const std::vector<std::vector<int>>& get_paths() {
    return paths;
  }

  bool check();

  ~VRPSolver() {}
};