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

double AnnealFacilitySolver::mock_close_facility(int i, std::vector<std::pair<int, int>>& moves) {
  if (assigned_count[i] == 0) return INF;
  double delta = -costs[i];

  std::vector<double> extra(facilities.size(), 0);
  std::vector<int> to_move;
  to_move.reserve(assigned_count[i]);
  for (int j = 0; j < consumers.size(); ++j) {
    if (assigned_facilities[j] == i) to_move.push_back(j);
  }
  std::sort(to_move.begin(), to_move.end(), [&](int a, int b){return demands[a] > demands[b];});

  for (int j : to_move) {
    int best = -1;
    double best_delta = INF;
    for (int k = 0; k < facilities.size(); ++k) {
      if (k == i || assigned_count[k] == 0) continue; // не трогаем закрытые магазины
      if (assigned_costs[k] + extra[k] + demands[j] > caps[k]) continue;
      double curr_delta = dist_matrix[k][j] - dist_matrix[i][j];
      if (curr_delta < best_delta) {
        best_delta = curr_delta;
        best = k;
      }
    }
    if (best == -1) return INF;
    delta += best_delta;
    extra[best] += demands[j];
    moves.push_back({j, best});
  }

  return delta;
}

void AnnealFacilitySolver::close_facility(int i, const std::vector<std::pair<int, int>>& moves) {
  for (auto [j, k] : moves) {
    assigned_costs[i] -= demands[j];
    --assigned_count[i];
    assigned_facilities[j] = k;
    assigned_costs[k] += demands[j];
    ++assigned_count[k];
  } 
}

double AnnealFacilitySolver::mock_open_facility(int i, std::vector<int>& lures) {
  if (assigned_count[i] > 0) return INF;
  double delta = costs[i];

  std::vector<std::pair<double, int>> benefits;
  benefits.reserve(consumers.size());
  for (int j = 0; j < consumers.size(); ++j) {
    benefits.push_back({dist_matrix[assigned_facilities[j]][j] - dist_matrix[i][j], j});
  }
  std::sort(benefits.begin(), benefits.end(), [](const std::pair<double, int>& a, const std::pair<double, int>& b){return a.first > b.first;});

  double used_cap = 0;
  std::vector<int> leaving(facilities.size(), 0);
  for (auto [b, j] : benefits) {
    if (used_cap + demands[j] > caps[i]) continue;
    int from = assigned_facilities[j];
    if (from == i) continue;
    double current_delta = b;
    if (leaving[from] + 1 == assigned_count[from]) current_delta += costs[from];
    if (current_delta <= 0) continue;
    used_cap += demands[j];
    delta -= current_delta;
    ++leaving[from];
    lures.push_back(j);
  }

  if (lures.empty()) return INF;

  return delta;
}

void AnnealFacilitySolver::open_facility(int i, const std::vector<int>& lures) {
  for (int j : lures) {
    int from = assigned_facilities[j];
    assigned_costs[from] -= demands[j];
    --assigned_count[from];
    assigned_facilities[j] = i;
    assigned_costs[i] += demands[j];
    ++assigned_count[i];
  }
}

bool AnnealFacilitySolver::apply_closes() {
  bool changed = false;
  for (int i = 0; i < facilities.size(); ++i) {
    if (assigned_count[i] == 0) continue;
    std::vector<std::pair<int, int>> moves;
    double delta = mock_close_facility(i, moves);
    if (delta != INF && delta < -EPS) {
      close_facility(i, moves);
      result += delta;
      changed = true;
    }
  }
  return changed;
}

bool AnnealFacilitySolver::apply_opens() {
  bool changed = false;
  for (int i = 0; i < facilities.size(); ++i) {
    if (assigned_count[i] > 0) continue;
    std::vector<int> lures;
    double delta = mock_open_facility(i, lures);
    if (delta != INF && delta < -EPS) {
      open_facility(i, lures);
      result += delta;
      changed = true;
    }
  }
  return changed;
}

bool AnnealFacilitySolver::apply_reassigns() {
  bool changed = false;
  for (int j = 0; j < consumers.size(); ++j) {
    int best = -1;
    double best_delta = -EPS;
    for (int i = 0; i < facilities.size(); ++i) {
      double d = mock_reassign(i, j);
      if (d != INF && d < best_delta) {
        best_delta = d;
        best = i;
      }
    }
    if (best != -1) {
      reassign(best, j);
      result += best_delta;
      changed = true;
    }
  }
  return changed;
}

void AnnealFacilitySolver::apply_all() {
  int max_iter = 30;
  while (max_iter > 0) {
    bool any = false;
    if (apply_closes()) any = true;
    if (apply_reassigns()) any = true;
    if (apply_opens()) any = true;
    if (!any) break;
    --max_iter;
  }
}

void AnnealFacilitySolver::solve_once(std::mt19937& rnd) {
  temp = start_temp;

  double run_best_result = result;
  std::vector<int> run_best_assigned = assigned_facilities;

  for (int k = 0; k < opt_count; ++k) {
    int r = rnd() % 1000;
    if (r < 3) {
      int i = rnd() % facilities.size();
      if (assigned_count[i] > 0) {
        std::vector<std::pair<int, int>> moves;
        double delta = mock_close_facility(i, moves);
        if (delta != INF && delta < temp * 10) {
          if (delta < 0 || (rnd() % 1000000000) / 1000000000.0 < std::exp(-delta / temp)) {
            close_facility(i, moves);
            result += delta;
          }
        }
      }
    } else if (r < 4) {
      int i = rnd() % facilities.size();
      if (assigned_count[i] == 0) {
        std::vector<int> lures;
        double delta = mock_open_facility(i, lures);
        if (delta != INF && delta < temp * 10) {
          if (delta < 0 || (rnd() % 1000000000) / 1000000000.0 < std::exp(-delta / temp)) {
            open_facility(i, lures);
            result += delta;
          }
        }
      }
    } else if (rnd() % 3 == 1) {
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

  assigned_costs.assign(facilities.size(), 0);
  assigned_count.assign(facilities.size(), 0);
  for (int j = 0; j < consumers.size(); ++j) {
    assigned_costs[assigned_facilities[j]] += demands[j];
    ++assigned_count[assigned_facilities[j]];
  }
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

    int iter = 5;
    while (iter > 0) {
      bool any = false;
      if (apply_closes()) any = true;
      if (apply_opens()) any = true;
      if (!any) break;
      --iter;
    }
    if (result < best_result) {
      best_result = result;
      best_assigned = assigned_facilities;
    }
  }

  result = best_result;
  assigned_facilities = best_assigned;
  assigned_costs.assign(facilities.size(), 0);
  assigned_count.assign(facilities.size(), 0);
  for (int j = 0; j < (int)consumers.size(); ++j) {
    assigned_costs[assigned_facilities[j]] += demands[j];
    assigned_count[assigned_facilities[j]]++;
  }

  apply_all();
}