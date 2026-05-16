#include "greedy_solver.h"

#include <vis.h>
#include <random>

class AnnealFacilitySolver : public GreedyFacilitySolver {
    std::vector<double> used;
    std::vector<bool> open;
  public:
    AnnealFacilitySolver(const std::vector<point>& facilities,
                         const std::vector<double>& costs,
                         const std::vector<double>& caps,
                         const std::vector<point>& consumers,
                         const std::vector<double>& demands) : GreedyFacilitySolver(facilities, costs, caps, consumers, demands) {}
    
    void solve() override;

    ~AnnealFacilitySolver() {}
  private:
    
    double mock_reassign(int i, int j);
    void reassign(int i, int j);
    double mock_swap_consumers(int i, int j);
    void swap_consumers(int i, int j);
    double mock_close_facility(int i, std::vector<std::pair<int, int>>& moves);
    void close_facility(int i, const std::vector<std::pair<int, int>>& moves);
    double mock_open_facility(int i, std::vector<int>& lures);
    void open_facility(int i, const std::vector<int>& lures);

    bool apply_closes();
    bool apply_opens();
    bool apply_reassigns();
    void apply_all();

    void solve_once(std::mt19937& rnd);
    void calc_dist_matrix();

    int restart_count = 25;
    int opt_count = 10000000;
    double start_temp = 50000;
    double end_temp = 1e-4;
    double temp, mul;

    const double INF = 1e18;

    std::vector<int> assigned_count;
    std::vector<double> assigned_costs;
    std::vector<std::vector<double>> dist_matrix;
};