#include "greedy_solver.h"

#include <iostream>

void GreedyTSPSolver::solve() {
    std::vector<bool> covered(points.size(), false);
    int current_index = 0;

    for (int _ = 1; _ < points.size(); ++_) {
        covered[current_index] = true;
        double best_distance;
        int best_index = -1;
        for (int i = 0; i < points.size(); ++i) {
            if (covered[i]) continue;
            double dist = TSPSolver::distance(points[current_index], points[i]);
            if (best_index == -1 || dist < best_distance) {
                best_distance = dist;
                best_index = i;
            }
        }
        result_set.push_back(current_index);
        current_index = best_index;
        result += best_distance;
    }
    result_set.push_back(current_index);
    result += TSPSolver::distance(points[current_index], points[0]);
}