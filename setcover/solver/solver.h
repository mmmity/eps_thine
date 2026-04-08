#pragma once

#include <vector>

class SSCSolver {
 protected:
  int n;
  std::vector<std::vector<int>> sets;
  std::vector<long long> costs;

  std::vector<int> result_set;
  long long result = -1;

 public:

  SSCSolver(int n, std::vector<std::vector<int>>&& sets, std::vector<long long>&& costs) : n(n), sets(std::move(sets)), costs(std::move(costs)) {}
  SSCSolver(int n, const std::vector<std::vector<int>>& sets, const std::vector<long long>& costs) : n(n), sets(sets), costs(costs) {}

  virtual void solve() {};

  long long get_result() {
    return result;
  }

  const std::vector<int>& get_result_set() {
    return result_set;
  }

  bool check();

  virtual ~SSCSolver() {}
};