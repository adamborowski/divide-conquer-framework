import modules.runner as runner

results = []
params = [1, 10, 100, 1000, 10000, 100000]
base_config = {
    'n': 6,
    'e': 10000,
    'o': None,
    'c': 10000,
    'i': 1000
}

for p in params:
    myRunner = runner.AggregatedDurationTester(runner.merge_dicts(base_config, {'p': p}))
    for i in range(0, 10):
        myRunner.run()
    results.append(myRunner.get_average())

print(results)
