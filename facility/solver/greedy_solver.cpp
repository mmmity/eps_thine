#include "greedy_solver.h"

void GreedyFacilitySolver::solve() {
    result = 0;
    used.resize(facilities.size(), 0);
    for (int i = 0; i < consumers.size(); ++i) {
        int nearest_free = -1;
        for (int j = 0; j < facilities.size(); ++j) {
            if (used[j] + demands[i] > caps[j]) continue;
            if (nearest_free == -1 || distance(facilities[j], consumers[i]) < distance(facilities[nearest_free], consumers[i])) {
                nearest_free = j;
            }
        }
        assigned_facilities[i] = nearest_free;
        result += distance(facilities[nearest_free], consumers[i]);
        used[nearest_free] += demands[i];
    }
    for (int i = 0; i < facilities.size(); ++i) {
        if (used[i] > 0) result += costs[i];
    }
}