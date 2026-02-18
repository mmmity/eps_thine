import os
import subprocess

with open('tests_for_evaluation.txt') as testnames_file:
  testnames = testnames_file.read().split('\n')

if len(testnames) == 0:
  testnames = os.listdir('tests')

with open('results.txt', 'w') as result_file:
  for name in testnames:  
    res = subprocess.run(['./build/setcover', f'tests/{name}'], capture_output=True)
    result_file.write(f'test {name}: {res.stdout.decode('utf-8')}\n')