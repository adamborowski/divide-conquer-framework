import modules.runner as runner

results = []
params = [1, 10, 100, 1000, 10000, 100000, 1000000]
params.reverse()
base_config = {
    'n': 6,
    'e': 1000,
    'o': None,
    'c': 10000
}

for initialQueueSize in params:
    myRunner = runner.AggregatedDurationTester(runner.merge_dicts(base_config, {'i': initialQueueSize}))
    for i in range(0, 10):
        myRunner.run()
    results.append(myRunner.get_average())

print(results)
