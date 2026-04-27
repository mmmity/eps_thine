#include <fstream>
#include <iomanip>
#include <iostream>
#include <greedy_solver.h>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "usage: ./vrp <test_filename> <result_filename>";
        return 1;
    }

    std::ifstream fin(argv[1]);
    int n, cars, capacity;
    fin >> n >> cars >> capacity;

    std::vector<double> demands(n);
    std::vector<VRPPoint> clients(n);

    for (int i = 0; i < n; ++i) {
        fin >> demands[i] >> clients[i].x >> clients[i].y;
    }

    GreedyVRPSolver solver(clients, demands, cars, capacity);

    solver.solve();
    if (!solver.check()) {
        std::cerr << "SOLVER DID NOT WORK AS INTENDED\n";
        return 1;
    }

    std::ofstream fout(argv[2]);
    fout << std::setprecision(10) << std::fixed << solver.get_result() << "\n";
    for (int i = 0; i < cars; ++i) {
        fout << solver.get_paths()[i].size() << " ";
        for (int j : solver.get_paths()[i]) fout << j << " ";
        fout << "\n";
    }

    std::cout << std::setprecision(10) << std::fixed << solver.get_result() << "\n";
}