#include "kmeans_solver.h"

#include <algorithm>
#include <iostream>
#include <numeric>
#include <tsp/solver/ls_solver.h>

bool KMeansVRPSolver::add_cluster(std::mt19937& rnd) {
    std::vector<int> possible_starts;
    for (int j = 1; j < clients.size(); ++j) {
        if (cluster[j] != -1) continue;
        possible_starts.push_back(j);
    }

    if (possible_starts.empty()) return true;

    int start = possible_starts[rnd() % possible_starts.size()];

    VRPPoint current_centroid = clients[start];
    cluster[start] = cluster_count++;
    int in_cluster = 1;
    double covered_demand = demands[start];
    while (true) {
        int added = -1;
        for (int j = 1; j < clients.size(); ++j) {
            if (cluster[j] != -1) continue;
            if (covered_demand + demands[j] > capacity) continue;
            if (added == -1 || distance(clients[j], current_centroid) < distance(clients[added], current_centroid)) {
                added = j;
            }
        }
        if (added == -1) return false;
        cluster[added] = cluster_count - 1;
        current_centroid.x = (in_cluster * current_centroid.x + clients[added].x) / (in_cluster + 1);
        current_centroid.y = (in_cluster * current_centroid.y + clients[added].y) / (in_cluster + 1);
        in_cluster++;
        covered_demand += demands[added];
    }
}

void KMeansVRPSolver::clusterize(std::mt19937& rnd) {
    cluster_count = 0;
    cluster.assign(clients.size(), -1);
    while (!add_cluster(rnd));
    if (cluster_count > cars) throw std::invalid_argument("too many clusters");
}

void KMeansVRPSolver::solve() {
    std::mt19937 rnd(1000-7);
    double best_result = std::numeric_limits<double>::max();
    std::vector<std::vector<int>> best_paths;

    for (int j = 0; j < 500; ++j) {
        try {
            result = 0;
            paths.assign(cars, std::vector<int>());

            clusterize(rnd);

            for (int i = 0; i < cars; ++i) {
                std::vector<point> clients_set;
                std::vector<int> indices;
                clients_set.emplace_back(clients[0].x, clients[0].y);
                indices.push_back(0);
                for (int j = 1; j < clients.size(); ++j) {
                    if (cluster[j] == i) {
                        clients_set.emplace_back(clients[j].x, clients[j].y);
                        indices.push_back(j);
                    }
                }
                LSTSPSolver tsp_solver(clients_set);
                // tsp_solver.set_vrp_constants();
                tsp_solver.solve();
                result += tsp_solver.get_result();
                std::vector<int> path = tsp_solver.get_result_set();
                int zero_ind = std::find(path.begin(), path.end(), 0) - path.begin();
                paths[i].clear();
                for (int j = zero_ind + 1; j < path.size(); ++j) paths[i].push_back(indices[path[j]]);
                for (int j = 0; j < zero_ind; ++j) paths[i].push_back(indices[path[j]]);
            }

            if (result < best_result) {
                best_result = result;
                best_paths = paths;
            }
        } catch(const std::invalid_argument&) {
            continue;
        }
    }

    result = best_result;
    paths = best_paths;
}
