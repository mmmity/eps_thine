#include "numeric.h"
#include <set>
#include <vector>

template <std::input_iterator It>
int mex(It begin, It end) {
  std::set<int> values;
  for (auto it = begin; it != end; it++) {
    values.insert(*it);
  }

  int next_value = 0;
  for (int i : values) {
    if (next_value != i) {
      break;
    }
    ++next_value;
  }
  return next_value;
}

template <std::input_iterator It>
int mex(It begin, It end, size_t max_value) {
  std::vector<bool> is_used(max_value + 1, false);
  for (auto it = begin; it != end; it++) {
    is_used[*it] = true;
  }

  for (int i = 0; i <= max_value; ++i) {
    if (!is_used[i]) return i;
  }

  return max_value + 1;
}

template int mex<std::vector<int>::iterator>(std::vector<int>::iterator, std::vector<int>::iterator);
template int mex<std::vector<int>::iterator>(std::vector<int>::iterator, std::vector<int>::iterator, size_t);
