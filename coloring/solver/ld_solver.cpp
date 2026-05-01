#include "ld_solver.h"
#include <numeric.h>
#include <vis.h>

#include <algorithm>

int LDColSolver::make_greedy_solution(const std::vector<int>& vertex_order, std::vector<int>& output_solution) {
  int n = graph.size();

  std::vector<int> coloring(n, -1);
  int max_col = 0;

  for (int i : vertex_order) {
    std::vector<int> neigh_colors;
    int current_max_color = 0;
    for (int u : graph[i]) {
      if (coloring[u] != -1) {
        current_max_color = std::max(current_max_color, coloring[u]);
        neigh_colors.push_back(coloring[u]);
      }
    }

    coloring[i] = mex(neigh_colors.begin(), neigh_colors.end(), current_max_color);
    max_col = std::max(max_col, coloring[i]);
  }
  
  output_solution = std::move(coloring);
  return max_col + 1;
}

int LDColSolver::optimize_current_solution(std::mt19937& rnd, std::vector<int>& current_solution) {
  int max_col = 0;
  for (int i : current_solution) max_col = std::max(max_col, i);

  // перемешиваем цвета в случайном порядке и сортируем вершины по цветам
  std::vector<int> color_order(max_col + 1);
  std::vector<int> color_sizes(max_col + 1, 0);
  for (int i : current_solution) {
    color_sizes[i]++;
  }

  std::iota(color_order.rbegin(), color_order.rend(), 0);
  std::shuffle(color_order.begin(), color_order.end(), rnd);

  std::vector<int> new_order;
  for (int col : color_order) {
    int col_start = new_order.size();
    for (int i = 0; i < graph.size(); ++i) {
      if (current_solution[i] == col) {
        new_order.push_back(i);
      }
    }
    // шаффлим вершины одного цвета чтобы больше разнообразия было
    std::shuffle(new_order.begin() + col_start, new_order.end(), rnd);
  }

  return make_greedy_solution(new_order, current_solution);
}

void LDColSolver::try_make_solution(std::mt19937& rnd) {
  std::vector<int> current_solution(graph.size());
  std::vector<int> basic_order(graph.size());
  std::iota(basic_order.begin(), basic_order.end(), 0);
  std::shuffle(basic_order.begin(), basic_order.end(), rnd);

  int res = make_greedy_solution(basic_order, current_solution);

  for (int i = 0; i < inner_max_steps; ++i) {
    res = optimize_current_solution(rnd, current_solution);
  }

  if (result == -1 || res < result) {
    result = res;
    result_col = std::move(current_solution);
  }
}

void LDColSolver::solve() {
  std::mt19937 rnd(seed);
  ProgressVisualizer vis(outer_max_steps);
  vis.start_visualization("Coloring local descent solver, outer steps");
  for (int i = 0; i < outer_max_steps; ++i) {
    try_make_solution(rnd);
    vis.add_progress(1);
  }
}