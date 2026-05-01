#include <vector>

class ColSolver {
 protected:
  std::vector<std::vector<int>> graph;

  std::vector<int> result_col;
  int result = -1;

 public:
  ColSolver(std::vector<std::vector<int>>&& graph) : graph(std::move(graph)) {}
  ColSolver(const std::vector<std::vector<int>>& graph) : graph(graph) {}

  virtual void solve() {};

  long long get_result() {
    return result;
  }

  const std::vector<int>& get_result_col() {
    return result_col;
  }

  bool check();

  virtual ~ColSolver() {}
};