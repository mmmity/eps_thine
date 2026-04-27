#include "greedy_solver.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <tsp/solver/greedy_solver.h>

void GreedyVRPSolver::solve() {
    result = 0;

    // хочу отсортировать по координате x, чтобы машины выбирали клиентов, близких друг к другу
    indices.resize(clients.size() - 1);
    std::iota(indices.begin(), indices.end(), 1);
    std::sort(indices.begin(), indices.end(), [&](int i, int j) {
        return clients[i].x < clients[j].x;
    });

    std::vector<std::vector<int>> clientIndices(cars);
    std::vector<bool> covered(clients.size(), false);
    for (int i = 0; i < cars; ++i) {
        double current_sum = 0;
        for (int j : indices) {
            if (covered[j]) continue;
            if (current_sum + demands[j] > capacity) continue;
            covered[j] = true;
            clientIndices[i].push_back(j);
            current_sum += demands[j];
        }
    }

    for (int i = 0; i < cars; ++i) {
        if (clientIndices[i].empty()) continue;
        std::vector<point> clients_set;
        clients_set.push_back({clients[0].x, clients[0].y});
        for (int j : clientIndices[i]) clients_set.push_back({clients[j].x, clients[j].y});
        GreedyTSPSolver tsp_solver(clients_set);
        tsp_solver.solve();
        result += tsp_solver.get_result();
        std::vector<int> path = tsp_solver.get_result_set();
        int zero_ind = std::find(path.begin(), path.end(), 0) - path.begin();
        for (int j = zero_ind + 1; j < path.size(); ++j) paths[i].push_back(clientIndices[i][path[j] - 1]);
        for (int j = 0; j < zero_ind; ++j) paths[i].push_back(clientIndices[i][path[j] - 1]);
    }
}