import modules.runner as runner

results = []
params = [1, 2, 3, 4, 5, 6, 7, 8]
base_config = {
    'n': 6,
    'e': 10000,
    'o': None,
    'c': 10000,
    'i': 1000,
    'p': 1000
}

for q in params:
    myRunner = runner.AggregatedDurationTester(runner.merge_dicts(base_config, {'q': q}))
    for i in range(0, 10):
        myRunner.run()
    results.append(myRunner.get_average())

print(results)
