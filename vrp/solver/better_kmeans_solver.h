#pragma once

#include "solver.h"
#include <random>
#include <vector>

class BetterKMeansVRPSolver : public VRPSolver {

  public:
    BetterKMeansVRPSolver(const std::vector<VRPPoint>& clients,
                          const std::vector<double>& demands,
                          int cars,
                          double capacity) : VRPSolver(clients, demands, cars, capacity) {}
    
    void solve() override;

    ~BetterKMeansVRPSolver() = default;

  private:
    std::vector<int> cluster;
    
    int cluster_count;
    bool clusterize(std::mt19937& rnd, int k);
    int calc_min_k();
};