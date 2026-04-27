#include <iomanip>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cmath>
#include <set>

struct point {
    double x, y;
};

double dist(point a, point b) {
    return std::hypot(a.x - b.x, a.y - b.y);
}

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage: ./checker <prefix> <testname>\n";
        return 1;
    }

    std::string prefix(argv[1]);
    std::string testname(argv[2]);

    std::ifstream inp(prefix + "/tests/" + testname);
    std::ifstream out(prefix + "/results/" + testname);
    if (!inp.is_open() || !out.is_open()) {
        std::cerr << std::filesystem::exists(prefix + "/tests/" + testname);
        std::cerr << std::filesystem::exists(prefix + "/results/" + testname);
        std::cerr << "\n";
        std::cerr << prefix << " " << testname << "\n";
        std::cerr << "Failed to open files\n";
        return 1;
    }
    int n, cars, capacity;
    inp >> n >> cars >> capacity;

    std::vector<double> demands(n);
    std::vector<point> clients(n);

    for (int i = 0; i < n; ++i) {
        inp >> demands[i] >> clients[i].x >> clients[i].y;
    }

    double given_ans;
    std::vector<std::vector<int>> paths(cars);
    out >> given_ans;
    for (int i = 0; i < cars; ++i) {
        int size;
        out >> size;
        paths[i].resize(size);
        for (int j = 0; j < size; ++j) out >> paths[i][j];
    }

    double actual_result = 0;
    std::vector<bool> satisfied(clients.size(), false);
    for (int i = 0; i < cars; ++i) {
        point prv = clients[0];
        double covered_demand = 0;
        for (int j : paths[i]) {
            actual_result += dist(prv, clients[j]);
            if (satisfied[j]) {
                std::cerr << "One client got visited twice\n";
                return 1;
            }
            satisfied[j] = true;
            prv = clients[j];
            covered_demand += demands[j];
        }
        actual_result += dist(prv, clients[0]);
        if (covered_demand > capacity) {
            std::cerr << "Car " << i << " exceeded capacity\n";
            return 1;
        }
    }

    for (int i = 1; i < clients.size(); ++i) {
        if (!satisfied[i]) {
            std::cerr << "Client " << i << " is not visited";
            return 1;
        }
    }

    if (abs(actual_result - given_ans) > 1e-6) {
        std::cerr << std::setprecision(10) << std::fixed;
        std::cerr << "Actual result " << actual_result << " does not match solver's result " << given_ans << "\n";
        return 1;
    }
    return 0;
}