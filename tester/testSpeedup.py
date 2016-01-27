import modules.runner as runner

results = []
params = [1, 2, 4, 8, 16, 32, 64, 128, 244, 256, 512]
base_config = {
    'e': 10000,
    'c': 100000,
    'p': 100,
    'f': 3,
    'q': 3,
    'i': 1000,
    'o': None
}

for p in params:
    myRunner = runner.AggregatedDurationTester(runner.merge_dicts(base_config, {'n': p}))
    for i in range(0, 10):
        myRunner.run()
    results.append(myRunner.get_average())

print(results)
