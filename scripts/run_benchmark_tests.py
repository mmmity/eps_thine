import os
import sys
import subprocess

from typing import Tuple

def calc_passed_threshold(value, small, big) -> Tuple[str, int]:
  if small < big:
    # if small < big, then we maximize
    if value < small:
      return "no", 0
    elif value < big:
      return "small", 3
    else:
      return "BIG", 5
  else:
    if value > small:
      return "no", 0
    elif value > big:
      return "small", 3
    else:
      return "BIG", 5
    # else minimize

BENCHMARK_TESTS = {
  'setcover': [
    ('sc_157_0', 130000, 94402),
    ('sc_330_0', 29, 24), 
    ('sc_1000_11', 240, 147),
    ('sc_5000_1', 70, 31),
    ('sc_10000_5', 120, 64),
    ('sc_10000_2', 280, 167),
  ],

  'knapsack': [
    ('ks_30_0', 92000, 99798),
    ('ks_50_0', 141956, 142156),
    ('ks_200_0', 100062, 100236),
    ('ks_400_0', 3966813, 3967028),
    ('ks_1000_0', 109869, 109899),
    ('ks_10000_0', 1099870, 1099881),
  ],

  'coloring': [
    ('gc_50_3', 8, 6),
    ('gc_70_7', 20, 17),
    ('gc_100_5', 21, 16),
    ('gc_250_9', 95, 78),
    ('gc_500_1', 18, 16),
    ('gc_1000_5', 124, 100),
  ]
}

for prefix, tests in BENCHMARK_TESTS.items():
  results = {}
  for test, _, _ in tests:
    print(f'Running test {prefix}/{test}: ', end='')
    sys.stdout.flush()
    res = subprocess.run([f'build/{prefix}', f'{prefix}/tests/{test}', f'{prefix}/results/{test}'], capture_output=True)
    print('OK')
    res_int = int(res.stdout.decode('utf-8').strip())
    results[test] = res_int
  
  with open(f'benchmark_results/{prefix}.txt', 'w') as f:
    s = 0
    for test, small, big in tests:
      typ, pts = calc_passed_threshold(results[test], small, big)
      s += pts
      f.write(f'{test} got {results[test]}, passing {typ} threshold and receiving {pts} points\n')
    f.write(f'sum points: {s}\n')