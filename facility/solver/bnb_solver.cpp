  #include "bnb_solver.h"

  #include <algorithm>
  #include <numeric>
  #include <vis.h>

  double BnBFacilitySolver::residue_upper_bound(int i) {
    // считаем что открыты все, кто помечен открытыми до i, а так же все кто после i
    // набираем жадно, выбирая для каждого человека ближайшую открытую и не переполненную

    double res = 0;
    std::vector<double> filled(costs.size(), 0);
    std::vector<bool> open(facilities.size(), false);
    for (int k : facility_indices) {
      open[k] = open_facilities[k];
      if (open[k]) res += costs[k];
    }

    for (int j = 0; j < consumers.size(); ++j) {
      int nearest_free = -1;
      double best_distance;
      for (int k = 0; k < facilities.size(); ++k) {
        if (!open_facilities[facility_indices[k]] && k < i) continue;
        if (filled[facility_indices[k]] + demands[j] > caps[facility_indices[k]]) continue;
        double dist = dist_matrix[j][facility_indices[k]];
        if (nearest_free == -1 || dist < best_distance) {
          nearest_free = facility_indices[k];
          best_distance = dist;
        }
      }
      if (nearest_free == -1) return std::numeric_limits<double>::max();
      filled[nearest_free] += demands[j];
      res += best_distance;
      if (!open[nearest_free]) {
        open[nearest_free] = true;
        res += costs[nearest_free];
      }
    }

    return res;
  }

  double BnBFacilitySolver::residue_lower_bound(int i) {
    // считаем что открыты все, кто помечен открытыми до i, и все после i
    // для каждого потребителя берем расстояние до ближайшего магазина
    // стоимость открытия дополнительных магазинов делаем как будто непрерывный рюкзак
    double res = 0;

    for (int j = 0; j < consumers.size(); ++j) {
      int nearest_free = -1;
      double best_distance;
      for (int k = 0; k < facilities.size(); ++k) {
        if (!open_facilities[facility_indices[k]] && k < i) continue;
        double dist = dist_matrix[j][facility_indices[k]];
        if (nearest_free == -1 || dist < best_distance) {
          nearest_free = facility_indices[k];
          best_distance = dist;
        }
      }
      res += best_distance;
    }

    double current_demand_cover = 0;
    for (int k = 0; k < facilities.size(); ++k) {
      if (k >= i) continue;
      if (open_facilities[facility_indices[k]]) {
        current_demand_cover += caps[facility_indices[k]];
        res += costs[facility_indices[k]];
      }
    }

    double needed_demand_cover = 0;
    for (double d : demands) needed_demand_cover += d;
    needed_demand_cover -= current_demand_cover;

    if (needed_demand_cover <= 0) return res;
    if (i >= facilities.size()) return std::numeric_limits<double>::max();

    for (int k = 0; k < facilities.size(); ++k) {
      if (k < i) continue;
      if (needed_demand_cover <= 0) return res;

      if (needed_demand_cover >= caps[facility_indices[k]]) {
        res += costs[facility_indices[k]];
        needed_demand_cover -= caps[facility_indices[k]];
      } else {
        res += costs[facility_indices[k]] * (needed_demand_cover / caps[facility_indices[k]]);
        return res;
      }
    }

    if (needed_demand_cover > 0) return std::numeric_limits<double>::max();
    return res;
  }

  void BnBFacilitySolver::greedy_finish(int i) {
    double res = 0;
    std::vector<double> filled(costs.size(), 0);
    std::vector<bool> open(facilities.size(), false);

    for (int j = 0; j < consumers.size(); ++j) {
      int nearest_free = -1;
      double best_distance;
      for (int k = 0; k < facilities.size(); ++k) {
        if (!open_facilities[facility_indices[k]] && k < i) continue;
        if (filled[facility_indices[k]] + demands[j] > caps[facility_indices[k]]) continue;
        double dist = dist_matrix[j][facility_indices[k]];
        if (nearest_free == -1 || dist < best_distance) {
          nearest_free = facility_indices[k];
          best_distance = dist;
        }
      }
      if (nearest_free == -1) return;
      assigned_facilities[j] = nearest_free;
      filled[nearest_free] += demands[j];
      res += best_distance;
      if (!open[nearest_free]) {
        open[nearest_free] = true;
        res += costs[nearest_free];
      }
    }

    result = res;
  }

  void BnBFacilitySolver::solve_recursive(int i) {
    if (i == facilities.size()) return;
    double ub = residue_upper_bound(i);
    double lb = residue_lower_bound(i);

    if (lb == std::numeric_limits<double>::max() || ub == std::numeric_limits<double>::max()) return;
    if (lb > best_ub) return;
    if (ub < best_ub) {
      best_ub = ub;
      best_mask = open_facilities;
      best_idx = i;
    }

    if (current_iter > iter_count) return;
    ++current_iter;
    vis.add_progress(1);

    open_facilities[facility_indices[i]] = true;
    solve_recursive(i + 1);
    open_facilities[facility_indices[i]] = false;
    solve_recursive(i + 1);
  }

  void BnBFacilitySolver::calc_dist_matrix() {
    dist_matrix.resize(consumers.size(), std::vector<double>());
    for (int i = 0; i < consumers.size(); ++i) {
      dist_matrix[i].resize(facilities.size());
      for (int j = 0; j < facilities.size(); ++j) {
        dist_matrix[i][j] = distance(consumers[i], facilities[j]);
      }
    }
  }

  void BnBFacilitySolver::choose_constants() {
    iter_count = 5000000000 / (consumers.size() * facilities.size());
  }

  void BnBFacilitySolver::solve() {
    choose_constants();
    vis.set_n(iter_count);
    int n = facilities.size();
    calc_dist_matrix();
    open_facilities.assign(n, false);
    facility_indices.resize(n);
    std::iota(facility_indices.begin(), facility_indices.end(), 0);
    std::sort(facility_indices.begin(), facility_indices.end(), [&](int a, int b) {
      return (caps[a] / costs[a]) > (caps[b] / costs[b]);
    });
    current_iter = 0;
    solve_recursive(0);

    open_facilities = best_mask;
    greedy_finish(best_idx);
  }