#include "solver.h"

#include <set>

bool FacilitySolver::check() {
    double actual_result = 0;
    std::set<int> open;
    for (int i : assigned_facilities) {
        open.insert(i);
    }

    for (int i : open) actual_result += costs[i];

    std::vector<double> used(caps.size(), 0);
    for (int i = 0; i < demands.size(); ++i) {
        used[assigned_facilities[i]] += demands[i];
        actual_result += distance(facilities[assigned_facilities[i]], consumers[i]);
    }

    for (int i : open) {
        if (used[i] > caps[i]) return false;
    }

    return std::abs(actual_result - result) < EPS;
}