#pragma once

#include <iomanip>
#include <iostream>
#include <math.h>
#include <string>
#include <chrono>

class ProgressVisualizer {
  public:
    ProgressVisualizer(int n, std::ostream& out = std::cout) : n(n), out(out) {}

    void start_visualization(const std::string msg) {
#ifndef NDEBUG
        start_time = std::chrono::steady_clock::now();
        current = 0;
        out << msg << std::endl;
        out << "\033[?25l";
        visualize_progress();
#endif
    }

    void add_progress(int delta) {
#ifndef NDEBUG
        current += delta;
        visualize_progress();
        if (current >= n) end_visualization();
#endif
    }
 
  private:

    void visualize_progress() {
        int filled = current * BAR_WIDTH / n;
        int percentage = current * 100 / n;
        if (filled > BAR_WIDTH || percentage > 100) throw std::out_of_range("progress bar stopped");
        out << "\r[";
        out << std::string(filled, '=') << std::string(BAR_WIDTH - filled, '-') << "] "
            << current << "/" << n << " "
            << percentage << "%";
        visualize_time();
        std::cout.flush();
    }

    void visualize_time() {
        auto elapsed = std::chrono::steady_clock::now() - start_time;
        out << std::fixed << std::setprecision(2);
        out << " " << std::chrono::duration<double>(elapsed).count() << "s";

        double speed = static_cast<double>(current) / std::chrono::duration<double>(elapsed).count();
        double remaining = speed > 0 ? (n - current) / speed : NAN;
        out << " (" << remaining << "s remaining)";
    }

    void end_visualization() {
        out << "\033[?25h";
        out << std::endl;
    }

    const int BAR_WIDTH = 30;
    int n;
    int current = 0;
    std::chrono::time_point<std::chrono::steady_clock> start_time;

    std::ostream& out;
};