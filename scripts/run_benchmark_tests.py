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
  # 'setcover': [
  #   ('sc_157_0', 130000, 94402),
  #   ('sc_330_0', 29, 24), 
  #   ('sc_1000_11', 240, 147),
  #   ('sc_5000_1', 70, 31),
  #   ('sc_10000_5', 120, 64),
  #   ('sc_10000_2', 280, 167),
  # ],

  # 'knapsack': [
  #   ('ks_30_0', 92000, 99798),
  #   ('ks_50_0', 141956, 142156),
  #   ('ks_200_0', 100062, 100236),
  #   ('ks_400_0', 3966813, 3967028),
  #   ('ks_1000_0', 109869, 109899),
  #   ('ks_10000_0', 1099870, 1099881),
  # ],

  # 'coloring': [
  #   ('gc_50_3', 8, 6),
  #   ('gc_70_7', 20, 17),
  #   ('gc_100_5', 21, 16),
  #   ('gc_250_9', 95, 78),
  #   ('gc_500_1', 18, 16),
  #   ('gc_1000_5', 124, 100),
  # ],

  # 'tsp': [
  #   ('tsp_51_1', 482, 430),
  #   ('tsp_100_3', 23433, 20800),
  #   ('tsp_200_2', 35985, 30000),
  #   ('tsp_574_1', 40000, 37600),
  #   ('tsp_1889_1', 378069, 323000),
  #   ('tsp_33810_1', 78478868, 67700000)
  # ],

  # 'facility': [
  #   ('fl_25_2', 4000000, 3269822),
  #   ('fl_100_1', 26000000, 22724634),
  #   ('fl_200_7', 5000000, 4711295),
  #   ('fl_500_7', 30000000, 27006099),
  #   ('fl_1000_2', 10000000, 8879294),
  #   ('fl_2000_2', 10000000, 7453531)
  # ],

  'vrp': [
    ('vrp_16_3_1', 387, 280),
    ('vrp_26_8_1', 1019, 630),
    ('vrp_51_5_1', 713, 540),
    ('vrp_101_10_1', 1193, 830),
    ('vrp_200_16_1', 3719, 1400),
    ('vrp_421_41_1', 2392, 2000)
  ]
}

for prefix, tests in BENCHMARK_TESTS.items():
  results = {}
  for test, _, _ in tests:
    print(f'Running test {prefix}/{test}: ', end='')
    sys.stdout.flush()
    res = subprocess.run([f'build/{prefix}', f'{prefix}/tests/{test}', f'{prefix}/results/{test}'], capture_output=True)
    print('OK')
    out = res.stdout.decode('utf-8').strip()
    if '.' in out:
      res_int = float(out)
    else:
      res_int = int(out)
    results[test] = res_int
  
  with open(f'benchmark_results/{prefix}.txt', 'w') as f:
    s = 0
    for test, small, big in tests:
      typ, pts = calc_passed_threshold(results[test], small, big)
      s += pts
      f.write(f'{test} got {results[test]}, passing {typ} threshold and receiving {pts} points\n')
    f.write(f'sum points: {s}\n')