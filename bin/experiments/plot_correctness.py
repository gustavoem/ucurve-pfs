import numpy as np
import matplotlib.pyplot as plt

file_name = "sffs_bfs_pucs_correctness.txt"

my_colors = ["#66c2a5", "#fc8d62", "#8da0cb", "#e78ac3"]
alg_color = {"PUCS": my_colors[0],
             "SFFS": my_colors[1],
             "BFS": my_colors[2],
             "ES": my_colors[3],
             "PFS": my_colors[0],
             "UBB": my_colors[1],
             "PPFS": my_colors[2],
             "RPFS": my_colors[3],
             "UBB-PFS": my_colors[2]}


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
    plt.plot (n, r, label = alg, color=alg_color[alg])
handlers, labels = ax.get_legend_handles_labels ()
ax.legend(handlers, labels, loc = 'best', numpoints = 1)
plt.xlabel ("Tamanho de instância")
plt.ylabel ("Proporção de melhor solução encontrada")

plt.savefig ('avg_correctness.pdf', bbox_inches='tight', transparent=True)
