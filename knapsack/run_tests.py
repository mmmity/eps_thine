import os
import subprocess

with open('tests_for_evaluation.txt') as testnames_file:
  tests_listed = testnames_file.read().strip()
  if tests_listed == '':
    testnames = os.listdir('tests')
  else:
    testnames = tests_listed.split('\n')

with open('results.txt', 'w') as result_file:
  for name in testnames:
    print(f'Running on test {name}: ', end='')
    res = subprocess.run(['./build/knapsack', f'tests/{name}'], capture_output=True)
    result_file.write(f'test {name}: {res.stdout.decode('utf-8')}\n')
    print('OK')