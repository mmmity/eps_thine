import os
import subprocess
import sys

TARGET_VALUES = ['setcover', 'knapsack', 'coloring']

if len(sys.argv) < 2 :
  print('usage: python3 run_tests.py <setcover|knapsack|coloring>')
  sys.exit(1)
target = sys.argv[1]

if target not in TARGET_VALUES:
  print('usage: python3 run_tests.py <setcover|knapsack|coloring>')
  sys.exit(1)

with open(f'{target}/tests_for_evaluation.txt') as testnames_file:
  tests_listed = testnames_file.read().strip()
  if tests_listed == '':
    testnames = os.listdir(f'{target}/tests')
  else:
    testnames = tests_listed.split('\n')

with open(f'{target}/results.txt', 'w') as result_file:
  for name in testnames:  
    print(f'Running on test {name}: ', end='')
    res = subprocess.run([f'build/{target}', f'{target}/tests/{name}'], capture_output=True)
    result_file.write(f'test {name}: {res.stdout.decode('utf-8')}\n')
    print('OK')