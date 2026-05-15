#include "ls_solver.h"

void LSSSCSolver::calc_covering() {
  covering.resize(n);
  for (int i = 0; i < sets.size(); ++i) {
    for (int j : sets[i]) covering[j].push_back(i);
  }

  int max_freq = 1;
  for (int i = 0; i < n; ++i) max_freq = std::max(max_freq, static_cast<int>(covering[i].size()));
  element_weights.resize(n);
  for (int i = 0; i < n; ++i) {
    // вес элемента - максимальная частота встречаемости деленная на количество покрывающих множеств
    element_weights[i] = 1.0 + static_cast<double>(max_freq) / std::max(1UL, covering[i].size());
  }
}

long long LSSSCSolver::calc_cost(const std::vector<int>& solution) {
  long long c = 0;
  for (int i : solution) c += costs[i];
  return c;
}

void LSSSCSolver::update_best(const std::vector<int>& solution) {
  long long c = calc_cost(solution);
  if (result == -1 || c < result) {
    result = c;
    result_set = solution;
  }
}

std::vector<int> LSSSCSolver::grasp(int top_k) {
  std::vector<bool> covered(n, false);
  std::vector<double> uncovered_weight(sets.size(), 0);

  for (int i = 0; i < sets.size(); ++i) {
    for (int j : sets[i]) uncovered_weight[i] += element_weights[j];
  }

  std::vector<int> solution;
  int yet_to_cover = n;
  while (yet_to_cover > 0) {
    std::vector<int> top;
    for (int i = 0; i < sets.size(); ++i) {
      if (uncovered_weight[i] <= EPS) continue;
      if (top.size() < top_k) {
        top.push_back(i);
        continue;
      }
      int worst = 0;
      for (int j = 0; j < top.size(); ++j) {
        if (uncovered_weight[worst] / costs[worst] > uncovered_weight[j] / costs[j]) worst = j;
      }
      if (uncovered_weight[i] / costs[i] > uncovered_weight[worst] / costs[worst]) top[worst] = i;
    }

    if (top.empty()) break;
    int chosen = top[rnd() % top.size()];
    solution.push_back(chosen);
    for (int i : sets[chosen]) {
      if (covered[i]) continue;
      covered[i] = true;
      --yet_to_cover;
      for (int j : covering[i]) {
        uncovered_weight[j] -= element_weights[i];
      }
    }
  }

  return solution;
}

void LSSSCSolver::fix_solution(std::vector<int>& solution) {
  std::vector<bool> covered(n, false);
  for (int i : solution) {
    for (int j : sets[i]) covered[j] = true;
  }

  std::vector<double> uncovered_weight(sets.size(), 0);
  int yet_to_cover = 0;
  for (int i = 0; i < n; ++i) {
    if (!covered[i]) ++yet_to_cover;
  }

  for (int i = 0; i < sets.size(); ++i) {
    for (int j : sets[i]) {
      if (!covered[j]) uncovered_weight[i] += element_weights[j];
    }
  }

  while (yet_to_cover > 0) {
    int best = -1;
    for (int i = 0; i < sets.size(); ++i) {
      if (uncovered_weight[i] <= EPS) continue;
      if (best == -1 || uncovered_weight[i] / costs[i] > uncovered_weight[best] / costs[best]) best = i;
    }
    if (best == -1) break;
    solution.push_back(best);
    for (int i : sets[best]) {
      if (covered[i]) continue;
      covered[i] = true;
      --yet_to_cover;
      for (int j : covering[i]) {
        uncovered_weight[j] -= element_weights[i];
      }
    }
  }
}

void LSSSCSolver::remove_redundant(std::vector<int>& solution) {
  std::vector<int> count(n, 0);
  for (int i : solution) {
    for (int j : sets[i]) ++count[j];
  }

  std::sort(solution.begin(), solution.end(), [&](int a, int b){return costs[a] > costs[b];});

  std::vector<int> not_removed;
  for (int i : solution) {
    bool remove = true;
    for (int j : sets[i]) {
      if (count[j] == 1) remove = false;
    }
    if (remove) {
      for (int j : sets[i]) --count[j];
    } else {
      not_removed.push_back(i);
    }
  }

  solution = std::move(not_removed);
}

void LSSSCSolver::solve() {
  auto solution = grasp(1);
  remove_redundant(solution);
  update_best(solution);

  // сначала без адаптивного обновления весов немножко покрутимся
  for (int p = 0; p * n * sets.size() < 100000000; ++p) {
    int top_k = 1 + rnd() % 5;
    auto current = grasp(top_k);

    remove_redundant(current);
    update_best(current);
    long long current_cost = calc_cost(current);
    int without_improve = 0;

    for (int t = 0; t * n < 400000; ++t) {
      auto s = current;
      int sz = s.size();
      int max_k = std::max(2, sz / 4);
      if (without_improve > 30) max_k = std::max(max_k, sz / 2);
      int k = 1 + rnd() % max_k;
      std::shuffle(s.begin(), s.end(), rnd);
      s.resize(sz - k);
      fix_solution(s);
      remove_redundant(s);

      long long c = calc_cost(s);
      if (c < current_cost) {
        current = std::move(s);
        current_cost = c;
        without_improve = 0;
        if (c < result) {
          result = c;
          result_set = current;
        }
      } else {
        if (rnd() % 100 < 5 && c <= current_cost + std::max(1LL, current_cost / 50)) {
          current = std::move(s);
          current_cost = c;
        }
        ++without_improve;
      }
      if (without_improve > 100) break;
    }
  }

  std::vector<int> count(n);
  for (int p = 0; p * n < 100000; ++p) {
    int top_k = 1 + rnd() % 5;
    auto current = grasp(top_k);

    // считаем сколько раз был покрыт элемент, и обновляем веса в зависимости от этого
    count.assign(n, 0);
    for (int i : current) {
      for (int j : sets[i]) ++count[j];
    }

    for (int i = 0; i < n; ++i) {
      if (count[i] >= 2) element_weights[i] *= 1.02;
      else element_weights[i] *= 0.97;

      element_weights[i] = std::max(element_weights[i], 0.5);
      element_weights[i] = std::min(element_weights[i], 20.0);
    }

    remove_redundant(current);
    update_best(current);
    long long current_cost = calc_cost(current);
    int without_improve = 0;

    for (int t = 0; t * n < 400000; ++t) {
      auto s = current;
      int sz = s.size();
      int max_k = std::max(2, sz / 4);
      if (without_improve > 30) max_k = std::max(max_k, sz / 2);
      int k = 1 + rnd() % max_k;
      std::shuffle(s.begin(), s.end(), rnd);
      s.resize(sz - k);
      fix_solution(s);
      remove_redundant(s);

      long long c = calc_cost(s);
      if (c < current_cost) {
        current = std::move(s);
        current_cost = c;
        without_improve = 0;
        if (c < result) {
          result = c;
          result_set = current;
        }
      } else {
        if (rnd() % 100 < 5 && c <= current_cost + std::max(1LL, current_cost / 50)) {
          current = std::move(s);
          current_cost = c;
        }
        ++without_improve;
      }
      if (without_improve > 100) break;
    }
  }
}