import os
import sys
import subprocess

PREFIXES = ['setcover', 'knapsack', 'coloring', 'tsp', 'facility']

def run_all_tests(prefix):
  print(f'Doing prefix {prefix}')

  test_files = os.listdir(f'{prefix}/tests')
  ct = 0
  ln = len(test_files)

  for test in test_files:
    ct += 1
    print(f'Checking on test {test} ({ct} out of {ln}): ', end='')
    sys.stdout.flush()

    _ = subprocess.run([f'build/{prefix}', f'{prefix}/tests/{test}', f'{prefix}/results/{test}'], capture_output=True)
    res = subprocess.run([f'build/{prefix}_checker', prefix, test], capture_output=True)

    if res.returncode != 0:
      print()
      print(f'Failed test {test}:\n{res.stderr.decode()}')
      sys.exit(1)
    
    print('OK')


for prefix in PREFIXES:
  run_all_tests(prefix)