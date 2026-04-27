#include "solver.h"

#include <iostream>
#include <iomanip>

bool VRPSolver::check() {
    double actual_result = 0;
    std::vector<bool> satisfied(clients.size(), false);
    for (int i = 0; i < cars; ++i) {
        VRPPoint prv = clients[0];
        double covered_demand = 0;
        for (int j : paths[i]) {
            actual_result += distance(prv, clients[j]);
            if (satisfied[j]) {
                std::cerr << "Client " << j << " got visited twice\n";
                return false;
            }
            satisfied[j] = true;
            prv = clients[j];
            covered_demand += demands[j];
        }
        actual_result += distance(prv, clients[0]);
        if (covered_demand > capacity) {
            return false;
        }
    }

    for (int i = 1; i < clients.size(); ++i) {
        if (!satisfied[i]) {
            std::cerr << "Client " << i << " is not visited";
            return false;
        }
    }

    if (abs(actual_result - result) > EPS) {
        std::cerr << std::setprecision(10) << std::fixed;
        std::cerr << "Actual result " << actual_result << " does not match solver's result " << result << "\n";
        return false;
    }
    return true;
}