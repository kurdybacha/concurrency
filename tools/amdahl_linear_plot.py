#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np
import os.path

def S(parallel, cpu_count):
    return cpu_count / float(cpu_count - cpu_count*parallel + parallel);

max_power = 16
cpu_count_range = range(1,2**max_power)

parallel_data = [
    (0.95, '--', 'green'),
    (0.90, '-.', 'purple'),
    (0.75, ':', 'red'),
    (0.50, '-', 'blue')
]

for parallel, linestyle, color in parallel_data:
    plt.plot(cpu_count_range,
             [S(parallel, cpu_count) for cpu_count in cpu_count_range],
             linestyle=linestyle,
             color=color)

tick_threshold = 13 # for readability
plt.xticks([2**i for i in range(1 if max_power < tick_threshold
                                  else tick_threshold, max_power+1)])

plt.ylabel("Speedup")
plt.xlabel("Number of processors")
plt.legend(["{}%".format(p*100) for p, _, _ in parallel_data],
           loc='upper right',
           title="Parallel portion")


current_file_name_no_extension = os.path.splitext(os.path.basename(__file__))[0]
plt.savefig(os.path.basename(current_file_name_no_extension + ".png"))
plt.show()
