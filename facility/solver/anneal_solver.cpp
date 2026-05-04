#include "anneal_solver.h"

double AnnealFacilitySolver::mock_reassign(int i, int j) {
  if (assigned_facilities[j] == i) return INF;
  if (assigned_costs[i] + demands[j] > caps[i]) return INF;
  double delta = 0;
  delta -= dist_matrix[assigned_facilities[j]][j];
  if (assigned_count[assigned_facilities[j]] == 1) delta -= costs[assigned_facilities[j]];
  if (assigned_count[i] == 0) delta += costs[i];
  delta += dist_matrix[i][j];
  return delta;
}

void AnnealFacilitySolver::reassign(int i, int j) {
  assigned_costs[assigned_facilities[j]] -= demands[j];
  assigned_count[assigned_facilities[j]]--;
  assigned_facilities[j] = i;
  assigned_count[i]++;
  assigned_costs[i] += demands[j];
}

double AnnealFacilitySolver::mock_swap_consumers(int i, int j) {
  if (assigned_facilities[i] == assigned_facilities[j]) return INF;
  if (assigned_costs[assigned_facilities[i]] - demands[i] + demands[j] > caps[assigned_facilities[i]]) return INF;
  if (assigned_costs[assigned_facilities[j]] - demands[j] + demands[i] > caps[assigned_facilities[j]]) return INF;
  double delta = 0;
  delta -= dist_matrix[assigned_facilities[i]][i];
  delta -= dist_matrix[assigned_facilities[j]][j];
  delta += dist_matrix[assigned_facilities[i]][j];
  delta += dist_matrix[assigned_facilities[j]][i];
  return delta;
}

void AnnealFacilitySolver::swap_consumers(int i, int j) {  
  assigned_costs[assigned_facilities[i]] -= demands[i];
  assigned_costs[assigned_facilities[j]] -= demands[j];
  assigned_costs[assigned_facilities[i]] += demands[j];
  assigned_costs[assigned_facilities[j]] += demands[i];

  std::swap(assigned_facilities[i], assigned_facilities[j]);
}

void AnnealFacilitySolver::solve_once(std::mt19937& rnd) {
  temp = start_temp;

  double run_best_result = result;
  std::vector<int> run_best_assigned = assigned_facilities;

  for (int k = 0; k < opt_count; ++k) {
    if (rnd() % 3 == 1) {
      int i = rnd() % facilities.size();
      int j = rnd() % consumers.size();
      double delta = mock_reassign(i, j);
      if (delta != INF && delta < temp * 10) {
        if (delta < 0 || (rnd() % 1000000000) / 1000000000.0 < std::exp(-delta / temp)) {
          reassign(i, j);
          result += delta;
        }
      }
    } else {
      int i = rnd() % consumers.size();
      int j = rnd() % consumers.size();
      double delta = mock_swap_consumers(i, j);
      if (delta != INF && delta < temp * 10) {
        if (delta < 0 || (rnd() % 1000000000) / 1000000000.0 < std::exp(-delta / temp)) {
          swap_consumers(i, j);
          result += delta;
        }
      }
    }
    temp *= mul;
    if (result < run_best_result) {
      run_best_result = result;
      run_best_assigned = assigned_facilities;
    }
  }

  result = run_best_result;
  assigned_facilities = run_best_assigned;
}

void AnnealFacilitySolver::calc_dist_matrix() {
  dist_matrix.resize(facilities.size());
  for (int i = 0; i < facilities.size(); ++i) {
    dist_matrix[i].resize(consumers.size());
    for (int j = 0; j < consumers.size(); ++j) {
      dist_matrix[i][j] = distance(facilities[i], consumers[j]);
    }
  }
}

void AnnealFacilitySolver::solve() {
  double max_cost = *std::max(costs.begin(), costs.end());
  start_temp = 1000000;
  calc_dist_matrix();
  mul = std::pow(end_temp / start_temp, 1.0 / opt_count);
  GreedyFacilitySolver::solve();
  std::vector<int> greedy_assigned = assigned_facilities;
  double greedy_result = result;
  std::vector<int> best_assigned = assigned_facilities;
  double best_result = result;

  std::mt19937 rnd(1000-7);
  for (int i = 0; i < restart_count; ++i) {
    assigned_facilities = greedy_assigned;
    result = greedy_result;
    assigned_costs.assign(facilities.size(), 0);
    assigned_count.assign(facilities.size(), 0);

    for (int j = 0; j < consumers.size(); ++j) {
      assigned_costs[assigned_facilities[j]] += demands[j];
      assigned_count[assigned_facilities[j]]++;
    }

    if (i > 0) {
      int mutations = consumers.size() / 20;
      for (int j = 0; j < mutations; ++j) {
        int f = rnd() % facilities.size();
        int c = rnd() % consumers.size();
        if (assigned_costs[f] + demands[c] <= caps[f] && assigned_facilities[c] != f) {
          double delta = mock_reassign(f, c);
          if (delta == INF) continue;
          reassign(f, c);
          result += delta;
        }
      }
    }

    solve_once(rnd);
    if (result < best_result) {
      best_result = result;
      best_assigned = std::move(assigned_facilities);
    }
  }

  result = best_result;
  assigned_facilities = std::move(best_assigned);
}