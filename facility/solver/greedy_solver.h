#include "solver.h"

class GreedyFacilitySolver : public FacilitySolver {
    std::vector<double> used;
    std::vector<bool> open;
  public:
    GreedyFacilitySolver(const std::vector<point>& facilities,
                         const std::vector<double>& costs,
                         const std::vector<double>& caps,
                         const std::vector<point>& consumers,
                         const std::vector<double>& demands) : FacilitySolver(facilities, costs, caps, consumers, demands) {}
    
    void solve() override;

    ~GreedyFacilitySolver() {}
  private:
    double penalty = 1e10;
    void try_least_facilities();
    void try_facilities_with_penalty();
    void try_facilities_with_opening_penalty();
    void try_nearest_facilities();
};