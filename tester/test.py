import modules.runner as runner

base_config = {
    'n': 6,
    'e': 10000,
    'c': 10000,
    'i': 1000,
    'p': 1000
}

result = runner.test_repeat(base_config, runner.merge_dicts({'o': None}, base_config), 10)
print(result)
