#include "solver.h"

#include <vis.h>

class BnBFacilitySolver : public FacilitySolver {
    std::vector<double> used;
    std::vector<bool> open;
  public:
    BnBFacilitySolver(const std::vector<point>& facilities,
                         const std::vector<double>& costs,
                         const std::vector<double>& caps,
                         const std::vector<point>& consumers,
                         const std::vector<double>& demands) : FacilitySolver(facilities, costs, caps, consumers, demands) {}
    
    void solve() override;

    ~BnBFacilitySolver() {}
  private:
    
    void solve_recursive(int i);
    double residue_upper_bound(int i);
    double residue_lower_bound(int i);
    void greedy_finish(int i);
    void calc_dist_matrix();
    void choose_constants();

    std::vector<int> facility_indices;
    std::vector<bool> open_facilities;
    std::vector<std::vector<double>> dist_matrix;
    double best_ub = std::numeric_limits<double>::max();
    std::vector<bool> best_mask;
    int best_idx;

    int iter_count;
    int current_iter = 0;
    ProgressVisualizer vis;
};