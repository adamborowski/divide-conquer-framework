import subprocess
import re


def merge_dicts(*dict_args):
    result = {}
    for dictionary in dict_args:
        result.update(dictionary)
    return result


def run(config):
    args = ""
    for key, value in config.items():
        if value is None:
            args += " -" + key
        else:
            args += " -" + key + " " + str(value)

    path = '/home/adam/.CLion12/system/cmake/generated/907752ba/907752ba/Debug/divide_conquer_framework'

    path_args = path + args
    print('running ' + path_args)
    p = subprocess.Popen([path_args], stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    out, err = p.communicate()
    decode = out.decode("utf-8")
    print(decode)
    return decode


def parse(s):
    return {
        'result': float(re.search("result: (.*)", s).group(1)),
        'duration': int(re.search("duration: (.*)", s).group(1)),
        'average_load': float(re.search("average load: (.*)", s).group(1)),
        'std_deviation': float(re.search("std deviation: (.*)", s).group(1))
    }


class AggregatedDurationTester:
    def __init__(self, config):
        self.sum = 0
        self.count = 0
        self.config = config

    def run(self):
        output = run(self.config)
        data = parse(output)
        self.count += 1
        self.sum += data['duration']

    def get_average(self):
        return self.sum / self.count


def test_repeat(base_config, optimized_config, num_repeat):
    base = AggregatedDurationTester(base_config)
    optimized = AggregatedDurationTester(optimized_config)
    for i in range(0, num_repeat):
        base.run()
        optimized.run()
    return {
        'base': base.get_average(),
        'optimized': optimized.get_average()
    }


base_config = {
    'n': 4,
    'e': 1000
}

result = test_repeat(base_config, merge_dicts({'o': None}, base_config), 10)
print(result)
