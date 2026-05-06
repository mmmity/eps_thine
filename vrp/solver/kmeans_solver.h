#pragma once

#include "solver.h"
#include <random>

class KMeansVRPSolver : public VRPSolver {

  public:
    KMeansVRPSolver(const std::vector<VRPPoint>& clients,
                    const std::vector<double>& demands,
                    int cars,
                    double capacity) : VRPSolver(clients, demands, cars, capacity) {}
    
    void solve() override;

    ~KMeansVRPSolver() {}

  private:
    std::vector<int> cluster;
    int cluster_count;

    void clusterize(std::mt19937& rnd);
    bool add_cluster(std::mt19937& rnd);
};