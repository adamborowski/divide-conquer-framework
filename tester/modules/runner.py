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
        'average_load': float(re.search("average thread load: (.*)", s).group(1)),
        'std_deviation': float(re.search("std deviation: (.*)", s).group(1))
    }


class AggregatedDurationTester:
    def __init__(self, config):
        self.sum = 0
        self.count = 0
        self.config = config
        self.lastResult = None
        self.exactResults = True

    def run(self):
        print("aggregated run: {}:".format(self.count))
        output = run(self.config)
        data = parse(output)
        self.count += 1
        self.sum += data['duration']
        _result = data['result']
        if self.lastResult is not None:
            if self.lastResult != _result:
                self.exactResults = False
        self.lastResult = _result

    def get_average(self):
        return self.sum / self.count




def test_repeat(base_config, optimized_config, num_repeat):
    base = AggregatedDurationTester(base_config)
    optimized = AggregatedDurationTester(optimized_config)
    for i in range(0, num_repeat):
        base.run()
        optimized.run()

    _result = False
    if base.exactResults and optimized.exactResults and base.lastResult == optimized.lastResult:
        _result = base.lastResult
    return {
        'base': base.get_average(),
        'optimized': optimized.get_average(),
        'speedup': 1 - optimized.get_average() / base.get_average(),
        'result': _result
    }
