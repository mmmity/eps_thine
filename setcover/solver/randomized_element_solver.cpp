#include "randomized_element_solver.h"
#include <vis.h>

#include <algorithm>
#include <math.h>
#include <random>
#include <queue>
#include <utility>

void RandomizedElementSSCSolver::greedy_finish() {
    int yet_to_cover = n;
    std::vector<bool> covered(n, false);
    for (int i : current_result_set) {
        for (int j : sets[i]) {
            if (!covered[j]) --yet_to_cover;
            covered[j] = true;
        }
    }

    std::vector<int> uncovered_count(sets.size(), 0);
    int unused = sets.size() - current_result_set.size();
    for (int i = 0; i < sets.size(); ++i) {
        for (int j : sets[i]) {
            if (!covered[j]) ++uncovered_count[i];
        }
    }

    while (yet_to_cover > 0 && unused > 0) {
        int best_set_idx = -1;
        int best_count = -1;
        long long best_cost = -1;
        for (int i = 0; i < sets.size(); ++i) {
            if (uncovered_count[i] == 0) continue;

            if (best_set_idx == -1 || uncovered_count[i] * best_cost > best_count * costs[i]) {
                best_set_idx = i;
                best_count = uncovered_count[i];
                best_cost = costs[i];
            }      
        }

        for (int i : sets[best_set_idx]) {
            if (!covered[i]) {
                covered[i] = true;
                for (int j : covering[i]) {
                    --uncovered_count[j];
                }
            }
        }
        yet_to_cover -= best_count;
        current_result += best_cost;
        current_result_set.push_back(best_set_idx);
    }

    if (yet_to_cover > 0) return;

    if (result == -1 || result > current_result) {
        result = current_result;
        result_set = current_result_set;
    }
}

void RandomizedElementSSCSolver::solve_for_sample(const std::vector<int>& sample) {
    int n = sample.size();
    int m = sets.size();

    std::vector<long long> dist(1 << n, std::numeric_limits<long long>::max());
    std::vector<int> best_set_for_mask(1 << n, -1);
    std::vector<int> parent(1 << n, -1);
    std::vector<int> parent_set(1 << n, -1);

    dist[0] = 0;
    std::priority_queue<std::pair<long long, int>,
                        std::vector<std::pair<long long, int>>,
                        std::greater<std::pair<long long, int>>> q;

    std::vector<int> covering_mask(m, 0);
    for (int i = 0; i < n; ++i) {
        for (int j : covering[sample[i]]) {
            covering_mask[j] |= (1 << i);
        }
    }

    for (int i = 0; i < m; ++i) {
        if (best_set_for_mask[covering_mask[i]] == -1) {
            best_set_for_mask[covering_mask[i]] = i;
        } else {
            if (costs[i] < costs[best_set_for_mask[covering_mask[i]]]) {
                covering_mask[best_set_for_mask[covering_mask[i]]] = -1;
                best_set_for_mask[covering_mask[i]] = i;
            } else {
                covering_mask[i] = -1;
            }
        }
    }

    std::vector<int> sets_to_enumerate;
    for (int i = 0; i < m; ++i) {
        if (covering_mask[i] != -1) sets_to_enumerate.push_back(i);
    }

    q.push(std::make_pair(0, 0));

    int ct = 0;
    std::vector<int> counts(1 << n, 0);

    std::vector<bool> used(1 << n, false);

    std::vector<std::vector<int>> current_covering(n);
    for (int i = 0; i < n; ++i) {
        for (int j : covering[sample[i]]) {
            if (covering_mask[j] != -1) current_covering[i].push_back(j);
        }
    }

    while (!q.empty()) {
        auto [cost, mask] = q.top();
        q.pop();
        if (used[mask]) continue;
        used[mask] = true;
        dist[mask] = cost;
        int ctz = __builtin_ctz(~mask);
        if (ctz >= n) continue;
        for (int i : current_covering[ctz]) {
            if (cost + costs[i] < dist[mask | covering_mask[i]]) {
                dist[mask | covering_mask[i]] = cost + costs[i];
                parent[mask | covering_mask[i]] = mask;
                parent_set[mask | covering_mask[i]] = i;
                q.push(std::make_pair(cost + costs[i], mask | covering_mask[i]));
            }
        }
    }
    if (dist[(1 << n) - 1] == std::numeric_limits<long long>::max()) return;
    if (result != -1 && result < dist[(1 << n) - 1]) return;

    current_result = dist[(1 << n) - 1];
    std::vector<int> this_result;
    int mask = (1 << n) - 1;
    while (mask > 0) {
        this_result.push_back(parent_set[mask]);
        mask = parent[mask];
    }
    current_result_set = this_result;
}

void RandomizedElementSSCSolver::solve_once() {
    std::vector<long long> min_cost(n, std::numeric_limits<int>::max());
    for (int i = 0; i < n; ++i) {
        for (int j : covering[i]) {
            min_cost[i] = std::min(min_cost[i], costs[j]);
        }
    }

    int to_solve = std::min(n, SAMPLE_SIZE);

    std::vector<int> sample(to_solve);
    for (int i = 0; i < to_solve; ++i) {
        std::discrete_distribution<int> dist(min_cost.begin(), min_cost.end());
        sample[i] = dist(rnd);
        min_cost[sample[i]] = 0;
    }

    solve_for_sample(sample);
    greedy_finish();
    vis.add_progress(1);
}

void RandomizedElementSSCSolver::solve() {
    vis.start_visualization("Randomized element SSC solver");
    for (int i = 0; i < MAX_STEPS; ++i) {
        // vis.add_progress(1);
        solve_once();
    }
}