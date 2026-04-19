#include <iomanip>
#include <iostream>
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
        std::cerr << "Failed to open files\n";
        return 1;
    }
    int n, m;
    inp >> n >> m;
    std::vector<double> costs(n), caps(n), demands(m);
    std::vector<point> facilities(n), consumers(m);

    for (int i = 0; i < n; ++i) {
        inp >> costs[i] >> caps[i] >> facilities[i].x >> facilities[i].y;
    }

    for (int i = 0; i < m; ++i) {
        inp >> demands[i] >> consumers[i].x >> consumers[i].y;
    }

    double given_ans;
    std::vector<int> assigned_facilities(m);
    out >> given_ans;
    for (int i = 0; i < m; ++i) {
        out >> assigned_facilities[i];
    }

    double actual_result = 0;
    std::set<int> open;
    for (int i : assigned_facilities) {
        if (i < 0 || i >= n) {
            std::cerr << "Wrong open facility index\n";
            return 1;
        }
        open.insert(i);
    }

    for (int i : open) {
        actual_result += costs[i];
    }

    std::vector<double> used(caps.size(), 0);
    for (int i = 0; i < demands.size(); ++i) {
        used[assigned_facilities[i]] += demands[i];
        actual_result += dist(facilities[assigned_facilities[i]], consumers[i]);
    }

    for (int i : open) {
        if (used[i] > caps[i]) {
            std::cerr << "Facility " << i << " exceeded capacity\n";
            return 1;
        }
    }

    std::cout << std::setprecision(10) << std::fixed;

    if (std::abs(actual_result - given_ans) >= 1e-6) {
        std::cerr << actual_result << " " << given_ans << "\n";
        std::cerr << "Result is wrong\n";
        return 1;
    };
    return 0;
}