#include <vector>

class KSSolver {
 protected:
  int W;
  std::vector<long long> weights;
  std::vector<long long> costs;

  std::vector<int> result_set;
  long long result;

 public:

  KSSolver(int W, std::vector<long long>&& weights, std::vector<long long>&& costs) : W(W), weights(std::move(weights)), costs(std::move(costs)) {}
  KSSolver(int W, const std::vector<long long>& weights, const std::vector<long long>& costs) : W(W), weights(weights), costs(costs) {}

  virtual void solve() {};

  long long get_result() {
    return result;
  }

  const std::vector<int>& get_result_set() {
    return result_set;
  }

  bool check();

  ~KSSolver() {}
};