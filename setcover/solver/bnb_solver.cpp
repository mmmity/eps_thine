#include "bnb_solver.h"
#include <algorithm>
#include <iostream>

void BnBSSCSolver::solve() {
    // vis.start_visualization("Branch'n'bound solver setcover");
    // indices.resize(sets.size());
    // std::iota(indices.begin(), indices.end(), 0);
    // std::sort(indices.begin(), indices.end(), [this](int i, int j) {
    //     return costs[i] / sets[i].size() < costs[j] / sets[j].size();
    // });

    // std::vector<int> rev_indices(sets.size());
    // for (int i = 0; i < sets.size(); ++i) {
    //     rev_indices[indices[i]] = i;
    // }

    // std::vector<std::vector<int>> tmp = sets;
    // std::vector<long long> tmpc = costs;
    // for (int i = 0; i < sets.size(); ++i) {
    //     sets[i].swap(tmp[indices[i]]);
    //     costs[i] = tmpc[indices[i]];
    // }

    element_freq.resize(n, 0);
    for (int i = 0; i < sets.size(); ++i) {
        for (int k : sets[i]) ++element_freq[k];
    }

    covering.resize(n);
    for (long long c : costs) current_result += c;
    for (int i = 0; i < sets.size(); ++i) {
      for (int j : sets[i]) covering[j].push_back(i);
    }

    try {
        solve_recursive(0);
    } catch (const EndOfBnB&) {
    } catch (const std::exception& e) {
        std::cout << "Solver caught an exception: " << e.what() << "\n";
    }
    if (!greedy_finish(current_state_end)) {
        std::cerr << "SOLVER DID NOT WORK AS INTENDED D\n";
        exit(1);
    }
    result = current_sure_cost;
    for (int i = 0; i < sets.size(); ++i) {
        if (current_sure_state[i]) result_set.push_back(i);
    }

    // for (int i = 0; i < sets.size(); ++i) {
    //     sets[i].swap(tmp[indices[i]]);
    //     costs[i] = tmpc[i];
    // }

    // std::vector<int> real_res_set = result_set;
    // for (int i = 0; i < real_res_set.size(); ++i) {
    //     real_res_set[i] = indices[real_res_set[i]];
    // }
    // result_set = real_res_set;

#ifndef NDEBUG
    for (int i : result_set) std::cout << i << " ";
#endif
}

void BnBSSCSolver::solve_recursive(int i) {
    if (i >= sets.size()) return;
    ++steps;
    if (steps > MAX_STEPS) throw EndOfBnB();
    // vis.add_progress(1);
    std::cout << steps << "\n";

    double lb = residue_lower_bound(i);
    if (lb == std::numeric_limits<double>::max() || lb >= current_result) {
        return;
    }

    if (i < sets.size() / 25 || i % 10 == 0) {
        // считаем ub только на некоторых слоях рекурсии чтобы побыстрее работало
        double ub = residue_upper_bound(i);
        std::cout << lb << " " << ub << "\n";
        if (ub == std::numeric_limits<double>::max()) {
            return;
        }


        if (ub < current_result) {
            current_result = ub;
            current_sure_cost = current_cost;
            current_sure_state = current_state;
            current_state_end = i;
        } 
    }
    // else if (static_cast<double>(ub - current_result) / ub > 0.05) {
    //     return;
    // }

    if (!check_useless(i)) {
        current_state[i] = true;
        current_cost += costs[i];
        for (int j : sets[i]) covered[j]++;
        if (current_cost < current_result) solve_recursive(i + 1);
        for (int j : sets[i]) covered[j]--;
        current_state[i] = false;
        current_cost -= costs[i];
    }
    solve_recursive(i + 1);
}

bool BnBSSCSolver::check_useless(int i) {
    for (int j : sets[i]) {
        if (covered[j] == 0) return false;
    }
    return true;
}

double BnBSSCSolver::residue_lower_bound(size_t i) {
    // решаем двойственную задачу к линейной релаксации
    std::vector<double> c(sets.size(), 0);
    for (int j = i; j < sets.size(); ++j) {
        c[j] = costs[j];
    }

    std::vector<bool> already_covered(n, false);
    for (int j = 0; j < i; ++j) {
        if (current_state[j]) {
            for (int k : sets[j]) already_covered[k] = true;
        }
    }

    std::vector<std::pair<double, int>> candidates;
    for (int j = i; j < sets.size(); ++j) {
        if (current_state[j]) continue;
        int newly_covered = 0;
        for (int k : sets[j]) if (!already_covered[k]) ++newly_covered;
        if (newly_covered > 0) candidates.emplace_back((double)costs[j] / newly_covered, j);
    }
    std::sort(candidates.begin(), candidates.end());

    double res = 0;
    for (auto& [ratio, idx] : candidates) {
        int newly_covered = 0;
        for (int k : sets[idx]) {
            if (!already_covered[k]) {
                already_covered[k] = true;
                ++newly_covered;
            }
        }
        if (newly_covered > 0) {
            res += costs[idx] * ((double)newly_covered / sets[idx].size());
        }
    }
    return res + current_cost;
}

bool BnBSSCSolver::greedy_finish(int i) {
    double res = 0;
    std::vector<bool> covered(n, false);
    size_t yet_to_cover = n;
    for (int j = 0; j < i; ++j) {
        if (current_sure_state[j]) {
            for (int k : sets[j]) {
                if (!covered[k]) --yet_to_cover;
                covered[k] = true;
            }
        }
    }

    size_t unused = sets.size() - i;

    std::vector<int> uncovered_count(sets.size(), 0);
    for (int j = i; j < sets.size(); ++j) {
        for (int k : sets[j]) {
            if (!covered[k]) ++uncovered_count[j];
        }
    }

    while (yet_to_cover > 0 && unused > 0) {
        int best_set_idx = -1;
        // double best_weighted = -1;
        long long best_cost = -1;
        int best_count = 0;
        for (int j = i; j < sets.size(); ++j) {
            if (current_sure_state[j]) continue;

            // double newly_weighted = 0;
            // for (int k : sets[j]) {
            //     newly_weighted += 1.0 / element_freq[k];
            // }
            // if (newly_weighted == 0) continue;

            int newly_covered = uncovered_count[j];

            if (best_set_idx == -1 || newly_covered * best_cost > best_count * costs[j]) {
                best_set_idx = j;
                // best_weighted = newly_weighted;
                best_cost = costs[j];
                best_count = uncovered_count[j];
            }      
        }

        if (best_set_idx == -1) {
            return false;
        }

        yet_to_cover -= best_count;
        current_sure_cost += best_cost;
        current_sure_state[best_set_idx] = true;
        for (int k : sets[best_set_idx]) {
            if (!covered[k]) {
                covered[k] = true;
                for (int t : covering[k]) {
                    --uncovered_count[t];
                }
            }
        }
        --unused;
    }

    return true;
}

double BnBSSCSolver::residue_upper_bound(size_t i) {
    size_t saved_cost = current_sure_cost;
    current_sure_cost = current_cost;
    std::vector<bool> saved_state = current_sure_state;
    current_sure_state = current_state;

    double ub;

    if (!greedy_finish(i)) ub = std::numeric_limits<double>::max();
    else ub = current_sure_cost;
    std::vector<bool> covered(n, false);
    size_t cnt = 0;
    for (int j = 0; j < sets.size(); ++j) {
        if (current_sure_state[j]) {
            for (int k : sets[j]) {
                if (!covered[k]) ++cnt;
                covered[k] = true;
            }
        }
    }

    current_sure_cost = saved_cost;
    current_sure_state = saved_state;
    if (cnt < n) return std::numeric_limits<double>::max();
    return ub;
}