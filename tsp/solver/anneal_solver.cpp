#include "anneal_solver.h"

double AnnealTSPSolver::mock_two_opt(int i, int j) {
  double mock_result = result;
  mock_result -= distance_matrix[result_set[i]][result_set[(i + 1) % points.size()]];
  mock_result -= distance_matrix[result_set[j]][result_set[(j + 1) % points.size()]];
  mock_result += distance_matrix[result_set[i]][result_set[j]];
  mock_result += distance_matrix[result_set[(i + 1) % points.size()]][result_set[(j + 1) % points.size()]];
  return mock_result;
}

void AnnealTSPSolver::two_opt(int i, int j) {
  result -= distance_matrix[result_set[i]][result_set[(i + 1) % points.size()]];
  result -= distance_matrix[result_set[j]][result_set[(j + 1) % points.size()]];
  std::reverse(result_set.begin() + i + 1, result_set.begin() + j + 1);
  for (int k = i + 1; k <= j; ++k) point_to_pos[result_set[k]] = k;
  result += distance_matrix[result_set[i]][result_set[(i + 1) % points.size()]];
  result += distance_matrix[result_set[j]][result_set[(j + 1) % points.size()]];
}

void AnnealTSPSolver::random_2opt(std::mt19937& rnd) {
  int i = rnd() % points.size();
  int j;
  if (nn_count > 0) j = point_to_pos[nearest_neighbors[result_set[i]][rnd() % nn_count]];
  else j = rnd() % points.size();
  if (i == j) return;
  if (i > j) std::swap(i, j);
  if (j == i + 1) return;
  double probability = (rnd() % 1000000000) / 1000000000.0;
  double new_result = mock_two_opt(i, j);
  double delta = new_result - result;
  if (delta < 0 || probability < std::exp(-delta / temp)) two_opt(i, j);
}

void AnnealTSPSolver::solve_once(std::mt19937& rnd) {
  temp = start_temp;
  for (int i = 0; i < opt_count; ++i) {
    random_2opt(rnd);
    temp *= mul;
  }
}

void AnnealTSPSolver::calc_nearest_neighbors() {
  nearest_neighbors.assign(points.size(), std::vector<int>(nn_count));
  for (int i = 0; i < points.size(); ++i) {
    std::vector<std::pair<float, int>> dist;

    for (int j = 0; j < points.size(); ++j) {
      if (i == j) continue;
      dist.emplace_back(distance_matrix[i][j], j);
    }
    
    std::nth_element(dist.begin(), dist.begin() + nn_count, dist.end(), [](const auto& a, const auto& b){return a.first < b.first;});
    nearest_neighbors[i].clear();
    for (int j = 0; j < nn_count; ++j) {
      nearest_neighbors[i].push_back(dist[j].second);
    }
  }
}

void AnnealTSPSolver::solve() {
  choose_constants();
  if (nn_count > 0) calc_nearest_neighbors();
  mul = std::pow(end_temp / start_temp, 1.0 / opt_count);
  GreedyTSPSolver::solve();

  point_to_pos.resize(points.size());
  for (int i = 0; i < points.size(); ++i) {
    point_to_pos[result_set[i]] = i;
  }
  std::vector<int> greedy_result_set = result_set;
  double greedy_result = result;
  std::mt19937 rnd(1000-7);

  double best_result = greedy_result;
  std::vector<int> best_result_set = greedy_result_set;

  for (int i = 0; i < restart_count; ++i) {
    result = greedy_result;
    result_set = greedy_result_set;
    solve_once(rnd);
    if (result < best_result) {
      best_result = result;
      best_result_set = std::move(result_set);
    }
  }

  result = best_result;
  result_set = std::move(best_result_set);
}

void AnnealTSPSolver::choose_constants() {
  if (points.size() < 10000) {
    restart_count = 1000;
    opt_count = 500000;
    start_temp = 10000;
    end_temp = 1e-4;
    nn_count = 20;
  } else {
    restart_count = 100;
    opt_count = 1000;
    start_temp = 1000;
    end_temp = 1e-4;
    nn_count = 10;
  }
}