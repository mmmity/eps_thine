#include <fstream>
#include <iomanip>
#include <iostream>
#include <anneal_solver.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage: ./facility <test_filename> <result_filename>";
        return 1;
    }

    std::ifstream fin(argv[1]);
    int n, m;
    fin >> n >> m;

    std::vector<double> costs(n), caps(n), demands(m);
    std::vector<point> facilities(n), consumers(m);

    for (int i = 0; i < n; ++i) {
        fin >> costs[i] >> caps[i] >> facilities[i].x >> facilities[i].y;
    }

    for (int i = 0; i < m; ++i) {
        fin >> demands[i] >> consumers[i].x >> consumers[i].y;
    }

    AnnealFacilitySolver solver(facilities, costs, caps, consumers, demands);

    solver.solve();
    if (!solver.check()) {
        std::cerr << "SOLVER DID NOT WORK AS INTENDED\n";
        return 1;
    }

    std::ofstream fout(argv[2]);
    fout << std::setprecision(10) << std::fixed << solver.get_result() << "\n";
    for (int i : solver.get_assigned_facilities()) {
        fout << i << " ";
    }

    std::cout << std::setprecision(10) << std::fixed << solver.get_result() << "\n";
}