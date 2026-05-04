#include "greedy_solver.h"

void GreedyFacilitySolver::try_least_facilities() {
    double res = 0;
    std::vector<int> current_assigned(consumers.size());
    used.assign(facilities.size(), 0);
    for (int i = 0; i < consumers.size(); ++i) {
        int best_free = -1;
        double best_cost;
        for (int j = 0; j < facilities.size(); ++j) {
            if (used[j] + demands[i] > caps[j]) continue;
            double cost = distance(facilities[j], consumers[i]);
            if (used[j] == 0) cost += penalty;
            if (best_free == -1 || cost < best_cost) {
                best_free = j;
            }
        }
        current_assigned[i] = best_free;
        res += distance(facilities[best_free], consumers[i]);
        if (used[best_free] == 0) res += costs[best_free];
        used[best_free] += demands[i];
    }

    if (result == 0 || res < result) {
        result = res;
        assigned_facilities = current_assigned;
    }
}

void GreedyFacilitySolver::try_facilities_with_opening_penalty() {
    double res = 0;
    std::vector<int> current_assigned(consumers.size());
    used.assign(facilities.size(), 0);
    for (int i = 0; i < consumers.size(); ++i) {
        int best_free = -1;
        double best_cost;
        for (int j = 0; j < facilities.size(); ++j) {
            if (used[j] + demands[i] > caps[j]) continue;
            double cost = distance(facilities[j], consumers[i]);
            if (used[j] == 0) cost += costs[j];
            if (best_free == -1 || cost < best_cost) {
                best_free = j;
                best_cost = cost;
            }
        }
        current_assigned[i] = best_free;
        res += best_cost;
        used[best_free] += demands[i];
    }

    if (result == 0 || res < result) {
        result = res;
        assigned_facilities = current_assigned;
    }
}

void GreedyFacilitySolver::try_facilities_with_penalty() {
    double res = 0;
    std::vector<int> current_assigned(consumers.size());
    used.assign(facilities.size(), 0);
    for (int i = 0; i < consumers.size(); ++i) {
        int best_free = -1;
        double best_cost;
        for (int j = 0; j < facilities.size(); ++j) {
            if (used[j] + demands[i] > caps[j]) continue;
            double cost = distance(facilities[j], consumers[i]) + costs[j] * (demands[i] / caps[j]);
            double curr_real_cost = distance(facilities[j], consumers[i]);
            if (best_free == -1 || cost < best_cost) {
                best_free = j;
                best_cost = cost;
            }
        }
        current_assigned[i] = best_free;
        res += distance(facilities[best_free], consumers[i]);
        if (used[best_free] == 0) res += costs[best_free];
        used[best_free] += demands[i];
    }

    if (result == 0 || res < result) {
        result = res;
        assigned_facilities = current_assigned;
    }
}

void GreedyFacilitySolver::try_nearest_facilities() {
    double res = 0;
    std::vector<int> current_assigned(consumers.size());
    used.assign(facilities.size(), 0);
    for (int i = 0; i < consumers.size(); ++i) {
        int best_free = -1;
        double best_cost;
        for (int j = 0; j < facilities.size(); ++j) {
            if (used[j] + demands[i] > caps[j]) continue;
            double cost = distance(facilities[j], consumers[i]);
            double curr_real_cost = distance(facilities[j], consumers[i]);
            if (best_free == -1 || cost < best_cost) {
                best_free = j;
                best_cost = cost;
            }
        }
        current_assigned[i] = best_free;
        res += distance(facilities[best_free], consumers[i]);
        if (used[best_free] == 0) res += costs[best_free];
        used[best_free] += demands[i];
    }

    if (result == 0 || res < result) {
        result = res;
        assigned_facilities = current_assigned;
    }
}

void GreedyFacilitySolver::solve() {
    try_facilities_with_opening_penalty();
    try_facilities_with_penalty();
    try_nearest_facilities();
    try_least_facilities();
}