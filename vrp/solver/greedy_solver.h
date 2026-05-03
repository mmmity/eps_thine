#pragma once

#include "solver.h"
#include <random>

class GreedyVRPSolver : public VRPSolver {
    std::vector<int> indices;
    int sorts_count = 500;

  public:
    GreedyVRPSolver(const std::vector<VRPPoint>& clients,
                    const std::vector<double>& demands,
                    int cars,
                    double capacity) : VRPSolver(clients, demands, cars, capacity) {}
    
    void solve_once(std::mt19937& rnd, bool by_x = false);
    void solve() override;

    ~GreedyVRPSolver() {}
};