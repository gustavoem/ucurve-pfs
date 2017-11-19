import matplotlib.pyplot as plt

file_name = "big"
# file_name = "small"
file_name = file_name + "_artificial_time_data.txt"

n = []
es_times = [[], []]
sfs_times = [[], []]
bfs_times = [[], []]
pucs_times = [[], []]

# n   ES              SFS             SFFS            PUCS             BFS       
# 1   0.004 ∓ 0.000   0.004 ∓ 0.000   0.004 ∓ 0.000   0.019 ∓  0.005   0.004 ∓  0.000
# 0   1     2 3       4     5 6       7     8 9       10    11 12      13    14 15
with open(file_name) as f:
    for line in f:
        v = line.split ()
        if (len (v) != 16):
            break
        n.append (int (v[0]))
        es_times[0].append (float (v[1]))
        es_times[1].append (float (v[3]))
        sfs_times[0].append (float (v[4]))
        sfs_times[1].append (float (v[6]))
        pucs_times[0].append (float (v[10]))
        pucs_times[1].append (float (v[12]))
        bfs_times[0].append (float (v[13]))
        bfs_times[1].append (float (v[15]))

results = {}
algorithms = [
    "PUCS",
    "SFS", 
    "BFS"]
    # "ES"]
results["ES"] = es_times
results["SFS"] = sfs_times
results["PUCS"] = pucs_times
results["BFS"] = bfs_times

# Plot data
ax = plt.gca () 
for alg in algorithms:
    r = results[alg]
    plt.errorbar (n, r[0], yerr = r[1], label = alg)
handlers, labels = ax.get_legend_handles_labels ()
handlers = [h[0] for h in handlers]
ax.legend(handlers, labels, loc = 'upper left', numpoints = 1)
plt.xlabel ("Instance size")
plt.ylabel ("Average execution time")
plt.title ("")
plt.show ()