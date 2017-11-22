import numpy as np
import matplotlib.pyplot as plt

file_name = "sffs_bfs_pucs_correctness.txt"


n = []
m = 50.0
sffs_corr = []
bfs_corr = []
pucs_corr = []

# n   SFFS BFS PUCS       
# 31  1    1   50
# 32  3    3   50
# 33  1    1   50
# 34  0    0   50
with open(file_name) as f:
    for line in f:
        v = line.split ()
        if (len (v) != 4):
            break
        n.append (int (v[0]))
        sffs_corr.append (float (v[1]) / m)
        bfs_corr.append (float (v[2]) / m)
        pucs_corr.append (float (v[3]) / m)

results = {}
algorithms = [
    "SFFS",
    "BFS",
    "PUCS"]
results["SFFS"] = sffs_corr
results["BFS"] = bfs_corr
results["PUCS"] = pucs_corr

# Plot data
ax = plt.gca () 
for alg in algorithms:
    r = results[alg]
    plt.plot (n, r, label = alg)
handlers, labels = ax.get_legend_handles_labels ()
ax.legend(handlers, labels, loc = 'best', numpoints = 1)
plt.xlabel ("Tamanho de instância")
plt.ylabel ("Proporção de melhor solução encontrada")
plt.show ()
