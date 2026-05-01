#include "greedy_solver.h"

#include <iostream>
#include <vis.h>

void GreedyTSPSolver::solve_from_start(int i) {
    std::vector<bool> covered(points.size(), false);
    int current_index = i;

    double current_result = 0;
    std::vector<int> current_result_set;

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
        current_result_set.push_back(current_index);
        current_index = best_index;
        current_result += best_distance;
    }
    current_result_set.push_back(current_index);
    current_result += TSPSolver::distance(points[current_index], points[i]);

    if (result < 0 || current_result < result) {
        result = current_result;
        result_set = std::move(current_result_set);
    }
}

void GreedyTSPSolver::solve() {
    ProgressVisualizer vis(points.size());
    for (int start = 0; start * points.size() * points.size() < 10000000000 && start < points.size(); ++start) {
        vis.add_progress(1);
        solve_from_start(start);
    }
}