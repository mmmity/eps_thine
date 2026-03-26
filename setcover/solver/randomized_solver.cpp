#include "randomized_solver.h"

#include <algorithm>

double RandomizedSSCSolver::lower_bound() {
    lb_weights.resize(sets.size());
    std::vector<double> current_costs(sets.size());
    for (int i = 0; i < sets.size(); ++i) current_costs[i] = costs[i];

    std::vector<bool> already_covered(n, false);

    double res = 0;
    for (int i = 0; i < n; ++i) {
        double min_cost = std::numeric_limits<double>::max();
        for (int k : covering[i]) {
            min_cost = std::min(min_cost, current_costs[k]);
        }

        if (min_cost == std::numeric_limits<double>::max()) {
            return 0;
        }
        res += min_cost; 
        for (int k : covering[i]) {
            current_costs[k] -= min_cost;
            lb_weights[k] += min_cost;
        }
    }
    return res;
}

void RandomizedSSCSolver::solve() {
    double lb = lower_bound();

    std::cout << lb << "\n";

    for (double& w : lb_weights) w /= lb;
    indices.resize(sets.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), [&](int i, int j) {
        return lb_weights[i] > lb_weights[j];
    });

    for (int _ = 0; _ < 5000; ++_) {
        std::cout << _ << "\n";
        solve_once();
    }
}

void RandomizedSSCSolver::remove_covered() {
    std::vector<int> covered(n, 0);
    for (int i : result_set) {
        for (int j : sets[i]) covered[j]++;
    }

    std::vector<bool> to_remove(sets.size(), false);
    for (int i : result_set) {
        bool useless = true;
        for (int j : sets[i]) if (covered[j] <= 1) useless = false;
        if (useless) {
            to_remove[i] = true;
            for (int j : sets[i]) covered[j]--;
        }
    }

    std::vector<int> new_result_set;
    for (int i : result_set) if (!to_remove[i]) new_result_set.push_back(i);
    result_set = std::move(new_result_set);
}

void RandomizedSSCSolver::solve_once() {
    double lambda = log(2 * n);
    std::vector<int> saved_result_set = get_result_set();
    size_t saved_result = get_result();

    result_set.clear();
    result = 0;

    for (int i = 0; i < sets.size(); ++i) {
        double prob = 0.5 + 0.5 * lb_weights[i];
        
        if (rnd() < prob * std::mt19937::max()) {
            result_set.push_back(i);
            result += costs[i];
        }
    }

    if (check()) {
        std::cout << "UR WEIRD\n";
    }
    remove_covered();

    if (check()) {
        if (result == -1 || result < saved_result) {
            std::cout << "FOUND SOMETHING\n";
            return;
        }
    }
    result_set = saved_result_set;
    result = saved_result;
}