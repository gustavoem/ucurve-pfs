import matplotlib.pyplot as plt


my_colors = ["#66c2a5", "#fc8d62", "#8da0cb", "#e78ac3", "#a6d854"]
alg_color = {"PUCS": my_colors[0],
             "SFFS": my_colors[1],
             "BFS": my_colors[2],
             "ES": my_colors[3],
             "PFS": my_colors[0],
             "UBB": my_colors[1],
             "PPFS": my_colors[2],
             "OPFS": my_colors[3],
             "UBB-PFS": my_colors[2],
             "PFS-RAND": my_colors[4]}

# file_name = "sffs_bfs_pucs_times.txt"
# file_name = "rpfs_pfs_times.txt"
# file_name = "ppfs_pfs_times.txt"
file_name = "ubb_pfs_ubb-pfs_times.txt"
# file_name = "pfs_rand_times.txt"



n = []
sffs_times = [[], []]
bfs_times = [[], []]
pucs_times = [[], []]
pfs_times = [[], []]
rpfs_times = [[], []]
ppfs_times = [[], []]
ubb_times = [[], []]
ubbpfs_times = [[], []]
pfs_rand_times = [[], []]

# n   SFFS            BFS             PUCS
# 0   1     2 3       4     5 6       7     8 9 
# 31  0.014 ∓ 0.008   0.007 ∓ 0.002   0.372 ∓ 0.352
# 32  0.012 ∓ 0.007   0.007 ∓ 0.002   0.443 ∓ 0.394
# 33  0.014 ∓ 0.007   0.007 ∓ 0.001   0.570 ∓ 0.538
# 34  0.014 ∓ 0.008   0.007 ∓ 0.001   0.440 ∓ 0.424

# n   PFS             OPFS
# 1   0.006 ∓ 0.006   0.005 ∓ 0.004 
# 2   0.005 ∓ 0.003   0.005 ∓ 0.002 
# 3   0.005 ∓ 0.003   0.005 ∓ 0.002 
# 4   0.005 ∓ 0.001   0.005 ∓ 0.001 
# 5   0.005 ∓ 0.002   0.005 ∓ 0.001

# n   PFS             PPFS
# 1   0.005 ∓ 0.004   0.028 ∓ 0.006
# 2   0.004 ∓ 0.000   0.038 ∓ 0.008
# 3   0.004 ∓ 0.000   0.039 ∓ 0.007
# 4   0.004 ∓ 0.000   0.047 ∓ 0.008
with open(file_name) as f:
    for line in f:
        v = line.split ()
        # if (len (v) != 7):
        #     break
        if (len (v) != 10):
            break
        n.append (int (v[0]))
        
        # sffs_times[0].append (float (v[1]))
        # sffs_times[1].append (float (v[3]))
        # bfs_times[0].append (float (v[4]))
        # bfs_times[1].append (float (v[6]))
        # pucs_times[0].append (float (v[7]))
        # pucs_times[1].append (float (v[9]))

        # pfs_times[0].append (float (v[1]))
        # pfs_times[1].append (float (v[3]))
        # rpfs_times[0].append (float (v[4]))
        # rpfs_times[1].append (float (v[6]))

        # pfs_times[0].append (float (v[1]))
        # pfs_times[1].append (float (v[3]))
        # ppfs_times[0].append (float (v[4]))
        # ppfs_times[1].append (float (v[6]))

        ubb_times[0].append (float (v[1]))
        ubb_times[1].append (float (v[3]))
        pfs_times[0].append (float (v[4]))
        pfs_times[1].append (float (v[6]))
        ubbpfs_times[0].append (float (v[7]))
        ubbpfs_times[1].append (float (v[9]))   

        # pfs_times[0].append (float (v[1]))
        # pfs_times[1].append (float (v[3]))
        # pfs_rand_times[0].append (float (v[4]))
        # pfs_rand_times[1].append (float (v[6]))     

results = {}
# algorithms = ["SFFS", "BFS", "PUCS"]
# algorithms = ["PFS", "OPFS"]
# algorithms = ["PFS", "PPFS"]
algorithms = ["UBB", "PFS", "UBB-PFS"]
# algorithms = ["PFS", "PFS-RAND"]
results["SFFS"] = sffs_times
results["BFS"] = bfs_times
results["PUCS"] = pucs_times
results["PFS"] = pfs_times
results["OPFS"] = rpfs_times
results["PPFS"] = ppfs_times
results["UBB"] = ubb_times
results["UBB-PFS"] = ubbpfs_times
results["PFS-RAND"] = pfs_rand_times

# Plot data
ax = plt.gca ()
for alg in algorithms:
    r = results[alg]
    plt.errorbar (n, r[0], yerr = r[1], label = alg, color=alg_color[alg])
handlers, labels = ax.get_legend_handles_labels ()
handlers = [h[0] for h in handlers]
ax.legend(handlers, labels, loc = 'upper left', numpoints = 1)
plt.xlabel ("Tamanho de instância")
# plt.xlabel ("Instance size")
plt.ylabel ("Tempo médio de execução")
# plt.ylabel ("Average execution time")
plt.title ("")

# plt.savefig ('opfs_avg_time.pdf', bbox_inches='tight', transparent=True)
# plt.savefig ('ppfs_avg_time.pdf', bbox_inches='tight', transparent=True)
# plt.savefig ('pucs_avg_time.pdf', bbox_inches='tight', transparent=True)
plt.savefig ('ubb-pfs_avg_time.pdf', bbox_inches='tight', transparent=True)
# plt.savefig ('pfs_rand_avg_time.pdf', bbox_inches='tight', transparent=True)