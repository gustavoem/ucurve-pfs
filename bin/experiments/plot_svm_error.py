import re
import numpy as np
import matplotlib.pyplot as plt

file_name = "xmeeting_results.txt"
data_sets_err = {}

# 10-run on     Iris with     pucs takes  0.091 and has cross-validation error of 0.067 
regex = r'\s*\d+-run\s+on\s+(\S+)\s+with.*error\sof\s(\d?\.?\d+)\s*'
i = 0
with open(file_name) as f:
    for line in f:
        m = re.search (regex, line)
        if (m):
            data_set = m.group (1)
            error = float (m.group (2))
            if (data_set not in data_sets_err):
                data_sets_err[data_set] = 0
            else:
                data_sets_err[data_set] += error
            i += 1
n = len (data_sets_err)
reps = i / n
names_arr = []
err_arr = []
for data_set in data_sets_err:
    name = str.title (re.sub ('_', ' ', data_set))
    names_arr.append (name)
    err_arr.append (data_sets_err[data_set] / reps)

print (names_arr)
print (err_arr)
print (n)

index = np.arange (n)
bar_width = 0.35
rects = plt.bar(index, err_arr, bar_width, label='Selected model')
plt.xlabel('Data set')
plt.ylabel('Average cross-validation error')
plt.title('')
plt.xticks(index, names_arr)

# plt.tight_layout()
plt.show()
