import modules.runner as runner


base_config = {
    'n': 8,
    'e': 10000,
    'c': 100000
}

result = runner.test_repeat(base_config, runner.merge_dicts({'o': None}, base_config), 10)
print(result)
