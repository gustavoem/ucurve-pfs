import re
import numpy as np
import matplotlib.pyplot as plt

file_name = "uci_experiments.txt"
algorithm_err = {}
my_colors = ["#bebada", "#fdae61", "#abd9e9"]

# 1-run on           Iris with     pucs takes  0.070 and has cross-validation error of 0.080 and selected  3 features
# 2-run on           Iris with     pucs takes  0.083 and has cross-validation error of 0.080 and selected  3 features
# 3-run on           Iris with     pucs takes  0.061 and has cross-validation error of 0.080 and selected  3 features
# 4-run on           Iris with     pucs takes  0.056 and has cross-validation error of 0.080 and selected  3 features
# 5-run on           Iris with     pucs takes  0.067 and has cross-validation error of 0.080 and selected  3 features
regex = r'^\s*\d+-run\s+on\s+(\S+)\s+with\s+(\S+).*error\sof\s(\d?\.?\d+)\s+and\s+selected\s+(\d+).*'
i = 0
with open(file_name) as f:
    for line in f:
        m = re.search (regex, line)
        if (m):
            data_set = m.group (1)
            algorithm = m.group (2)
            error = float (m.group (3))
            
            if (algorithm not in algorithm_err):
                algorithm_err[algorithm] = {}

            data_sets_err = algorithm_err[algorithm]
            if (data_set not in data_sets_err):
                data_sets_err[data_set] = error
            else:
                data_sets_err[data_set] += error
            i += 1

n_algs = len (algorithm_err)
n = len (next (iter (algorithm_err.values())))
reps = i / (n * n_algs)
names_arr = []
alg_err_arrays = {}

# get datasets names
for data_set in next (iter (algorithm_err.values())):
    name = str.title (re.sub ('_', ' ', data_set))
    names_arr.append (name)

# set average error of each algorithm for each dataset
for algorithm in algorithm_err:
    data_sets_err = algorithm_err[algorithm]
    for data_set in data_sets_err:
        if (algorithm not in alg_err_arrays):
            alg_err_arrays[algorithm] = [data_sets_err[data_set] / reps]
        else:
            alg_err_arrays[algorithm].append (data_sets_err[data_set] / reps)


n_range = np.arange (n)
ones = np.ones (n)

axes = plt.gca ()
axes.set_ylim([0, 1])

# creates bars
bar_width = 1
i = 0
for algorithm in alg_err_arrays:
    err_arr = alg_err_arrays[algorithm]
    indexes = n_range * (n_algs + 1) + (ones * (i % n_algs))
    print (algorithm)
    print (err_arr)
    print (indexes)
    bar_label = "características do " + algorithm
    if algorithm == "all":
        bar_label = "todas características"
    rects = plt.bar (indexes, err_arr, bar_width, label=bar_label, color=my_colors[i + 1])
    plt.legend ()
    i += 1

plt.xlabel ('Conjunto de dados')
plt.ylabel ('Erro médio de validação cruzada')
plt.title ('')
plt.xticks (n_range * (n_algs + 1) + (n_algs / 4) * ones, names_arr,rotation=45)


plt.savefig ('svm_error.pdf', bbox_inches='tight')
