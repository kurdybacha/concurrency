from matplotlib import pyplot as plt
import numpy as np
import os.path

fig = plt.figure()

x = ["One queue","Queue per thread","Task stealing"]
y = [10,5,1]

plt.ylim(0, max(y) + 1)
plt.xlim(-.5, 3)
plt.yticks(range(max(y) + 1))

for i in range(len(x)):
    rect = plt.bar(x[i], 0, color='c', width=0.5)
    current_file_name_no_extension = os.path.splitext(os.path.basename(__file__))[0]
    plt.savefig(os.path.basename(
        current_file_name_no_extension + str(i) + "_label.png"))
    rect = plt.bar(x[i], y[i], color='c', width=0.5)
    plt.savefig(os.path.basename(
        current_file_name_no_extension + str(i) + "_result.png"))

plt.show()
