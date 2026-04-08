#include "bnb_solver.h"
#include <algorithm>
#include <iostream>

void BnBKSSolver::solve() {
    vis.start_visualization("Branch'n'bound solver setcover");

    std::vector<int> indices(weights.size());
    std::iota(indices.begin(), indices.end(), 0);
    std::sort(indices.begin(), indices.end(), 
              [this](int a, int b) {
                return costs[a] * weights[b] > costs[b] * weights[a];
              });

    std::vector<long long> new_weights(weights.size());
    std::vector<long long> new_costs(costs.size());
    for (int i = 0; i < weights.size(); ++i) {
        new_weights[i] = weights[indices[i]];
        new_costs[i] = costs[indices[i]];
    }

    std::vector<long long> saved_weights = std::move(weights);
    std::vector<long long> saved_costs = std::move(costs);

    costs = std::move(new_costs);
    weights = std::move(new_weights);

    try {
        solve_recursive(0);
    } catch (const BnBKSSolver::EndOfBnB&) {
    } catch (const std::exception& e) {
        std::cout << "Solver caught an exception: " << e.what() << "\n";
    }
    if (!greedy_finish(current_state_end)) {
        std::cerr << "SOLVER DID NOT WORK AS INTENDED D\n";
        exit(1);
    }

    costs = std::move(saved_costs);
    weights = std::move(saved_weights);
    std::vector<bool> changed_sure_state = std::move(current_sure_state);
    current_sure_state.resize(weights.size());
    for (int i = 0; i < weights.size(); ++i) {
        current_sure_state[indices[i]] = changed_sure_state[i];
    }

    result = current_sure_cost;
    for (int i = 0; i < weights.size(); ++i) {
        if (current_sure_state[i]) result_set.push_back(i);
    }
}

void BnBKSSolver::solve_recursive(int i) {
    if (i >= weights.size()) return;
    ++steps;
    if (steps > MAX_STEPS) throw EndOfBnB();
    vis.add_progress(1);
    // std::cout << steps << "\n";

    double lb = residue_lower_bound(i);
    double ub = residue_upper_bound(i);
    if (ub <= current_result) {
        return;
    }

    if (lb > current_result) {
        current_result = lb;
        current_sure_cost = current_cost;
        current_sure_weight = current_weight;
        current_sure_state = current_state;
        current_state_end = i;
    }

    if (current_weight + weights[i] > W) {
        solve_recursive(i + 1);
        return;
    }

    double forward_lb_false = residue_lower_bound(i + 1);
    current_state[i] = true;
    current_cost += costs[i];
    current_weight += weights[i];
    double forward_lb_true = residue_lower_bound(i + 1);
    // std::cout << forward_lb_false << " " << forward_lb_true << "\n";
    if (forward_lb_false > forward_lb_true) {
        current_state[i] = false;
        current_cost -= costs[i];
        current_weight -= weights[i];
        solve_recursive(i + 1);
        current_state[i] = true;
        current_cost += costs[i];
        current_weight += weights[i];
        solve_recursive(i + 1);
        current_state[i] = false;
        current_cost -= costs[i];
        current_weight -= weights[i];
    } else {
        solve_recursive(i + 1);
        current_state[i] = false;
        current_cost -= costs[i];
        current_weight -= weights[i];
        solve_recursive(i + 1);
    }
    return;
}

long long BnBKSSolver::residue_lower_bound(size_t start) {
    int w = current_weight;
    long long max_ans = 0;
    long long sum_ans = 0;
    for (int i = start; i < weights.size(); ++i) {
        if (current_weight + weights[i] <= W) max_ans = std::max(max_ans, costs[i]);
        if (w + weights[i] <= W) {
            sum_ans += costs[i];
            w += weights[i];
        }
    }

    return std::max(max_ans, sum_ans) + current_cost;
}

bool BnBKSSolver::greedy_finish(int start) {
    int w = current_sure_weight;
    long long max_ans = 0;
    long long sum_ans = 0;
    std::vector<int> sum_indices;
    int max_idx;
    for (int i = start; i < weights.size(); ++i) {
        if (current_sure_weight + weights[i] <= W) {
            if (max_ans < costs[i]) {
                max_ans = costs[i];
                max_idx = i;
            }
        }
        if (w + weights[i] <= W) {
            sum_ans += costs[i];
            w += weights[i];
            sum_indices.push_back(i);
        }
    }

    if (max_ans < sum_ans) {
        for (int i : sum_indices) current_sure_state[i] = true;
        current_sure_cost += sum_ans;
    } else {
        current_sure_state[max_idx] = true;
        current_sure_cost += max_ans;
    }

    return true;
}

double BnBKSSolver::residue_upper_bound(size_t start) {
    double w = current_weight;
    double ans = 0;
    for (int i = start; i < weights.size(); ++i) {
        if (w + weights[i] <= W) {
            ans += costs[i];
            w += weights[i];
        } else {
            double left = W - w;
            ans += costs[i] * left / weights[i];
            break;
        }
    }

    return ans + current_cost;
}