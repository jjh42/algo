"""Simple script to benchmark btrees of various sizes."""

T_RANGE=range(2, 101)
PROBLEM_SIZE = 10000000
N_REPETITIONS = 20
MAP_CHOICES=["hash", "btree24", "red_black"]

import numpy as np
import resource
import os


def benchmark_btree(t, problem_size):
    """Determine the user CPU time for btree with t value and problem size."""
    start_time = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime
    os.system('./benchmark_map btree%d %d' % (t, problem_size))
    end_time = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime
    return end_time - start_time


def benchmark_map(map_choice, problem_size):
    """Determine the user CPU time for btree with t value and problem size."""
    start_time = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime
    os.system('./benchmark_map %s %d' % (map_choice, problem_size))
    end_time = resource.getrusage(resource.RUSAGE_CHILDREN).ru_utime
    return end_time - start_time


def main():
    for m in MAP_CHOICES:
        timings = [benchmark_map(m, PROBLEM_SIZE)
                   for i in range(N_REPETITIONS)]
        print ('%s\t%f\t%f' % (m, np.mean(timings), np.std(timings)))


if __name__ == '__main__':
    main()
