import numpy as np
import matplotlib.pyplot as plt

file_name = "big"
# file_name = "small"
file_name = file_name + "_artificial_corr_data.txt"

n = []
es_corr = []
sfs_corr = []
bfs_corr = []
pucs_corr = []

# n   ES SFS  SFFS PUCS BFS       
# 1   20 20   20   20   20
# 0   1  2    3    4    5
with open(file_name) as f:
    for line in f:
        v = line.split ()
        if (len (v) != 6):
            break
        n.append (int (v[0]))
        m = float (v[1])
        es_corr.append (float (v[1]) / m)
        sfs_corr.append (float (v[2]) / m)
        pucs_corr.append (float (v[4]) / m)
        bfs_corr.append (float (v[5]) / m)

results = {}
algorithms = [
    "PUCS",
    "SFS",
    "BFS"]
    # "ES"]
results["ES"] = es_corr
results["SFS"] = sfs_corr
results["PUCS"] = pucs_corr
results["BFS"] = bfs_corr

# Plot data
ax = plt.gca () 
for alg in algorithms:
    r = results[alg]
    plt.plot (n, r, label = alg)
handlers, labels = ax.get_legend_handles_labels ()
ax.legend(handlers, labels, loc = 'best', numpoints = 1)
plt.xlabel ("Instance size")
plt.ylabel ("Best solution proportion")
plt.show ()