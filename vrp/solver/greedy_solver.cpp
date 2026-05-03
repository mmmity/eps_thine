#include "greedy_solver.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <tsp/solver/anneal_solver.h>

void GreedyVRPSolver::solve_once(std::mt19937& rnd, bool by_x) {
    // хочу отсортировать по случайному вектору, чтобы машины выбирали клиентов, близких друг к другу
    indices.resize(clients.size() - 1);
    double x, y;
    if (by_x) {
        x = 1;
        y = 0;
    } else {
        x = (rnd() % 20000 - 10000) / 10000.0;
        y = (rnd() % 20000 - 10000) / 10000.0;
    }
    std::iota(indices.begin(), indices.end(), 1);
    std::sort(indices.begin(), indices.end(), [&](int i, int j) {
        return clients[i].x * x + clients[i].y * y < clients[j].x * x + clients[j].y * y;
    });

    std::vector<std::vector<int>> clientIndices(cars);
    std::vector<bool> covered(clients.size(), false);
    int uncovered = clients.size() - 1;
    for (int i = 0; i < cars; ++i) {
        double current_sum = 0;
        for (int j : indices) {
            if (covered[j]) continue;
            if (current_sum + demands[j] > capacity) continue;
            covered[j] = true;
            clientIndices[i].push_back(j);
            --uncovered;
            current_sum += demands[j];
        }
    }

    if (uncovered > 0) return;

    result = 0;

    for (int i = 0; i < cars; ++i) {
        if (clientIndices[i].empty()) continue;
        std::vector<point> clients_set;
        clients_set.push_back({clients[0].x, clients[0].y});
        for (int j : clientIndices[i]) clients_set.push_back({clients[j].x, clients[j].y});
        LSTSPSolver tsp_solver(clients_set);
        // tsp_solver.set_vrp_constants();
        tsp_solver.solve();
        result += tsp_solver.get_result();
        std::vector<int> path = tsp_solver.get_result_set();
        int zero_ind = std::find(path.begin(), path.end(), 0) - path.begin();
        paths[i].clear();
        for (int j = zero_ind + 1; j < path.size(); ++j) paths[i].push_back(clientIndices[i][path[j] - 1]);
        for (int j = 0; j < zero_ind; ++j) paths[i].push_back(clientIndices[i][path[j] - 1]);
    }
}

void GreedyVRPSolver::solve() {
    double best_result = std::numeric_limits<double>::max();
    std::vector<std::vector<int>> best_paths;

    std::mt19937 rnd(1000-7);

    for (int i = 0; i < sorts_count; ++i) {
        solve_once(rnd, i == 0);
        if (result != 0 && best_result > result) {
            best_result = result;
            best_paths = paths;
        }
    }

    paths = best_paths;
    result = best_result;
}