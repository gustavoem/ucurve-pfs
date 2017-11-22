import matplotlib.pyplot as plt

file_name = "sffs_bfs_pucs_times.txt"

n = []
sffs_times = [[], []]
bfs_times = [[], []]
pucs_times = [[], []]

# n   SFFS            BFS             PUCS
# 0   1     2 3       4     5 6       7     8 9 
# 31  0.014 ∓ 0.008   0.007 ∓ 0.002   0.372 ∓ 0.352
# 32  0.012 ∓ 0.007   0.007 ∓ 0.002   0.443 ∓ 0.394
# 33  0.014 ∓ 0.007   0.007 ∓ 0.001   0.570 ∓ 0.538
# 34  0.014 ∓ 0.008   0.007 ∓ 0.001   0.440 ∓ 0.424
with open(file_name) as f:
    for line in f:
        v = line.split ()
        if (len (v) != 10):
            break
        n.append (int (v[0]))
        sffs_times[0].append (float (v[1]))
        sffs_times[1].append (float (v[3]))
        bfs_times[0].append (float (v[4]))
        bfs_times[1].append (float (v[6]))
        pucs_times[0].append (float (v[7]))
        pucs_times[1].append (float (v[9]))
        

results = {}
algorithms = ["SFFS", "BFS", "PUCS"]
results["SFFS"] = sffs_times
results["BFS"] = bfs_times
results["PUCS"] = pucs_times

# Plot data
ax = plt.gca () 
for alg in algorithms:
    r = results[alg]
    plt.errorbar (n, r[0], yerr = r[1], label = alg)
handlers, labels = ax.get_legend_handles_labels ()
handlers = [h[0] for h in handlers]
ax.legend(handlers, labels, loc = 'upper left', numpoints = 1)
plt.xlabel ("Tamanho de instância")
plt.ylabel ("Tempo médio de execução")
plt.title ("")
plt.show ()
