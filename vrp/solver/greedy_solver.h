#pragma once

#include "solver.h"

class GreedyVRPSolver : public VRPSolver {
    std::vector<int> indices;

  public:
    GreedyVRPSolver(const std::vector<VRPPoint>& clients,
                    const std::vector<double>& demands,
                    int cars,
                    double capacity) : VRPSolver(clients, demands, cars, capacity) {}
    
    void solve() override;

    ~GreedyVRPSolver() {}
};