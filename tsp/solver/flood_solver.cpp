#include "flood_solver.h"

#include <iostream>

void FloodTSPSolver::solve() {
    choose_constants();
    std::mt19937 rnd(1000 - 7);
    candidates.reserve(candidate_count);
    for (int i = 0; i < candidate_count - 1; ++i) {
        candidates.emplace_back(points);
        candidates.back().shuffle(rnd);
    }
    
    candidates.emplace_back(points);
    candidates.back().GreedyTSPSolver::solve();

    std::vector<double> initial_results;
    initial_results.reserve(candidate_count);
    for (auto& c : candidates) {
        initial_results.push_back(c.get_result());
    }

    int idx = candidate_count / 2;
    std::nth_element(initial_results.begin(), initial_results.begin() + idx, initial_results.end());
    double current_level = initial_results[idx];

    std::vector<int> active_indices(candidate_count);
    for (int i = 0; i < candidate_count; ++i) {
        active_indices[i] = i;
    }

    double global_best_result = candidates.back().get_result();
    int best_candidate_idx = candidate_count - 1;
    while (!active_indices.empty()) {
        int idx = 0;
        while (idx < active_indices.size()) {
            int i = active_indices[idx];
            for (int k = 0; k < times_for_2opt; ++k) {
                candidates[i].random_2opt(rnd);
            }
            
            double current_res = candidates[i].get_result();
            
            if (current_res < global_best_result) {
                global_best_result = current_res;
                best_candidate_idx = i;
            }
            if (current_res >= current_level) {
                active_indices[idx] = active_indices.back();
                active_indices.pop_back();
            } else {
                ++idx;
            }
        }
        current_level -= increase;
    }

    result = candidates[best_candidate_idx].get_result();
    result_set = candidates[best_candidate_idx].get_result_set();
}

void FloodTSPSolver::choose_constants() {
  if (points.size() < 100) {
    candidate_count = 10000;
    increase = 5;
    times_for_2opt = 40;
  } else if (points.size() < 500) {
    candidate_count = 500;
    increase = 10;
    times_for_2opt = 30;
  } else if (points.size() < 1000) {
    candidate_count = 500;
    increase = 10;
    times_for_2opt = 10;
  } else if (points.size() < 5000) {
    candidate_count = 50;
    increase = 20;
    times_for_2opt = 10;
  } else {
    candidate_count = 1;
    increase = 40;
    times_for_2opt = 5;
  }
}