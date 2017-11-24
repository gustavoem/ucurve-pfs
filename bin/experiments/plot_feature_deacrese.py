import re
import numpy as np
import matplotlib.pyplot as plt

file_name = "uci_experiments.txt"
algorithm_feats = {}
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
            # error = float (m.group (3))
            selected_feats = int (m.group (4))
            if (algorithm not in algorithm_feats):
                algorithm_feats[algorithm] = {}

            data_sets_feats = algorithm_feats[algorithm]
            if (data_set not in data_sets_feats):
                data_sets_feats[data_set] = selected_feats
            else:
                data_sets_feats[data_set] += selected_feats
            i += 1

n_algs = len (algorithm_feats)
n = len (next (iter (algorithm_feats.values())))
reps = i / (n * n_algs)
names_arr = []
avg_alg_feats_arr = {}

# get datasets names
for data_set in next (iter (algorithm_feats.values())):
    name = str.title (re.sub ('_', ' ', data_set))
    names_arr.append (name)

# set average error of each algorithm for each dataset
for algorithm in algorithm_feats:
    data_sets_feats = algorithm_feats[algorithm]
    for data_set in data_sets_feats:
        if (algorithm not in avg_alg_feats_arr):
            avg_alg_feats_arr[algorithm] = [data_sets_feats[data_set] / reps]
        else:
            avg_alg_feats_arr[algorithm].append (data_sets_feats[data_set] / reps)


# creates bars
bar_width = 1.2
n_range = np.arange (n) * 2
i = 0

axes = plt.gca ()
axes.set_ylim([0, 80])

avg_feats_arr = avg_alg_feats_arr["all"]
bar_label = "todas características"
rects = plt.bar (n_range, avg_feats_arr, bar_width, label=bar_label, color=my_colors[2])
plt.legend (loc="upper left")

bar_width = 1
n_range = np.arange (n) * 2
avg_feats_arr = avg_alg_feats_arr["pucs"]
indexes = n_range
bar_label = "características do " + "pucs"
rects = plt.bar (n_range, avg_feats_arr, bar_width, label=bar_label, color=my_colors[1])
plt.legend (loc="upper left")

plt.xlabel ('Conjunto de dados')
plt.ylabel ('Número médio de características selecionadas')
plt.title ('')
plt.xticks (n_range, names_arr,rotation=45)

plt.savefig ('avg_features.pdf', bbox_inches='tight')
