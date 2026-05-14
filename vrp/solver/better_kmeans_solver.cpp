#include "better_kmeans_solver.h"
#include <knapsack/solver/bnb_solver.h>
#include <tsp/solver/ls_solver.h>

#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>
#include <limits>

bool BetterKMeansVRPSolver::clusterize(std::mt19937& rnd, int k) {
    cluster_count = k;
    cluster.assign(clients.size(), -1);
    
    std::vector<VRPPoint> centroids(k);
    std::vector<int> client_indices(clients.size() - 1);
    std::iota(client_indices.begin(), client_indices.end(), 1);

    std::vector<int> shuffled_for_centroids = client_indices;
    std::shuffle(shuffled_for_centroids.begin(), shuffled_for_centroids.end(), rnd);
    for (int i = 0; i < k; ++i) {
        centroids[i] = clients[shuffled_for_centroids[i]];
    }

    std::vector<int> sorted_clients = client_indices; 
    std::sort(sorted_clients.begin(), sorted_clients.end(), [&](int a, int b) {
        return demands[a] > demands[b];
    });

    int unassigned = clients.size();
    for (int j = 0; j < 10; ++j) {
        cluster.assign(clients.size(), -1);
        std::vector<double> current_capacity(k, 0);
        std::vector<int> cluster_sizes(k, 0);
        std::vector<VRPPoint> sum_centroids(k, {0, 0});

        for (int i : sorted_clients) {
            int best_cluster = -1;
            double best_dist = std::numeric_limits<double>::max();

            for (int c = 0; c < k; ++c) {
                if (current_capacity[c] + demands[i] <= capacity) {
                    double dist = distance(clients[i], centroids[c]);
                    if (dist < best_dist) {
                        best_dist = dist;
                        best_cluster = c;
                    }
                }
            }

            if (best_cluster == -1) {
                return false; 
            }

            cluster[i] = best_cluster;
            current_capacity[best_cluster] += demands[i];
            
            sum_centroids[best_cluster].x += clients[i].x;
            sum_centroids[best_cluster].y += clients[i].y;
            cluster_sizes[best_cluster]++;
            unassigned--;
        }

        for (int c = 0; c < k; ++c) {
            if (cluster_sizes[c] > 0) {
                centroids[c].x = sum_centroids[c].x / cluster_sizes[c];
                centroids[c].y = sum_centroids[c].y / cluster_sizes[c];
            } else {
                centroids[c] = clients[client_indices[rnd() % client_indices.size()]]; 
            }
        }
    }

    return true;
}

void BetterKMeansVRPSolver::solve() {
    std::mt19937 rnd(1000-7);
    double best_result = std::numeric_limits<double>::max();
    std::vector<std::vector<int>> best_paths;

    double sum_demand = 0;
    for (size_t i = 1; i < clients.size(); ++i) {
        sum_demand += demands[i];
    }
    int min_k = std::max(1, static_cast<int>(std::ceil(sum_demand / capacity)));

    for (int j = 0; j < 500; ++j) {
        result = 0;
        paths.assign(cars, std::vector<int>());

        int k = min_k;

        while (!clusterize(rnd, k)) {
            ++k;
            if (k > cars) break;    // пытаемся использовать минимальное число машин
        }

        if (k > cars) continue;

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
    }

    result = best_result;
    paths = best_paths;
}