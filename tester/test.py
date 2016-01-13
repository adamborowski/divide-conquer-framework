import modules.runner as runner

base_config = {
    'n': 1,
    'e': 4000,
}

result = runner.test_repeat(base_config, runner.merge_dicts({'o': None}, base_config), 10)
print(result)
