import modules.runner as runner

results = []
params = [1, 10, 100, 1000, 10000, 100000, 1000000, 10000000]
params.reverse()
base_config = {
    'n': 6,
    'e': 10000,
    'o': None
}

for c in params:
    myRunner = runner.AggregatedDurationTester(runner.merge_dicts(base_config, {'c': c}))
    for i in range(0, 10):
        myRunner.run()
    results.append(myRunner.get_average())

print(results)
