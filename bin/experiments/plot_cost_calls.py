import matplotlib.pyplot as plt


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

# file_name = "sffs_bfs_pucs_calls.txt"
# file_name = "rpfs_pfs_calls.txt"
# file_name = "ppfs_pfs_calls.txt"
file_name = "ubb_pfs_ubb-pfs_calls.txt"


n = []
sffs_calls = [[], []]
bfs_calls = [[], []]
pucs_calls = [[], []]
pfs_calls = [[],[]]
rpfs_calls = [[],[]]
ppfs_calls = [[],[]]
ubb_calls = [[],[]]
ubbpfs_calls = [[],[]]

# n   SFFS            BFS             PUCS
# 0   1     2 3       4     5 6       7     8 9 
# 31  0.014 ∓ 0.008   0.007 ∓ 0.002   0.372 ∓ 0.352
# 32  0.012 ∓ 0.007   0.007 ∓ 0.002   0.443 ∓ 0.394
# 33  0.014 ∓ 0.007   0.007 ∓ 0.001   0.570 ∓ 0.538
# 34  0.014 ∓ 0.008   0.007 ∓ 0.001   0.440 ∓ 0.424

# n   PFS             RPFS
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
        
        # sffs_calls[0].append (float (v[1]))
        # sffs_calls[1].append (float (v[3]))
        # bfs_calls[0].append (float (v[4]))
        # bfs_calls[1].append (float (v[6]))
        # pucs_calls[0].append (float (v[7]))
        # pucs_calls[1].append (float (v[9]))

        # pfs_calls[0].append (float (v[1]))
        # pfs_calls[1].append (float (v[3]))
        # rpfs_calls[0].append (float (v[4]))
        # rpfs_calls[1].append (float (v[6]))

        # pfs_calls[0].append (float (v[1]))
        # pfs_calls[1].append (float (v[3]))
        # ppfs_calls[0].append (float (v[4]))
        # ppfs_calls[1].append (float (v[6]))

        ubb_calls[0].append (float (v[1]))
        ubb_calls[1].append (float (v[3]))
        pfs_calls[0].append (float (v[4]))
        pfs_calls[1].append (float (v[6]))
        ubbpfs_calls[0].append (float (v[7]))
        ubbpfs_calls[1].append (float (v[9]))        

results = {}
# algorithms = ["SFFS", "BFS", "PUCS"]
# algorithms = ["PFS", "RPFS"]
# algorithms = ["PFS", "PPFS"]
algorithms = ["UBB", "PFS", "UBB-PFS"]
results["SFFS"] = sffs_calls
results["BFS"] = bfs_calls
results["PUCS"] = pucs_calls
results["PFS"] = pfs_calls
results["RPFS"] = rpfs_calls
results["PPFS"] = ppfs_calls
results["UBB"] = ubb_calls
results["UBB-PFS"] = ubbpfs_calls

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
plt.ylabel ("Média de chamadas a função de custo")
# plt.ylabel ("Average cost function calls")
plt.title ("")

# plt.savefig ('rpfs_avg_calls.pdf', bbox_inches='tight', transparent=True)
# plt.savefig ('ppfs_avg_calls.pdf', bbox_inches='tight', transparent=True)
# plt.savefig ('pucs_avg_calls.pdf', bbox_inches='tight', transparent=True)
plt.savefig ('ubb-pfs_avg_calls.pdf', bbox_inches='tight', transparent=True)