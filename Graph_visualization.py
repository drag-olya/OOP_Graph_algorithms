import matplotlib.pyplot as plt
import networkx as nx

files = input()

init_graph, res_graph = files.split()[0], files.split()[1]

with open(init_graph, "r") as f1:
    lines1 = f1.readlines()

if res_graph == 'FordFulkerson.txt':
    G1 = nx.DiGraph()
    G2 = nx.DiGraph()
else:
    G1 = nx.Graph()
    G2 = nx.Graph()

if len(lines1[0])-1 == 1:  # there is a matrix in the file
    for i, line in enumerate(lines1[1:]):
        data = line.split()
        for j, weight in enumerate(data):
            if float(weight) > 0:
                G1.add_edge(i, j, weight=float(weight))
else:
    for line in lines1:
        data = line.split()
        node1 = int(data[0])
        node2 = int(data[1])
        weight = float(data[2])
        G1.add_edge(node1, node2, weight=weight)


with open(res_graph, "r") as f2:
    lines2 = f2.readlines()


for line in lines2:
    data = line.split()
    node1 = int(data[0])
    node2 = int(data[1])
    weight = float(data[2])
    G2.add_edge(node1, node2, weight=weight)

title = res_graph.replace('.txt', '')
fig, ax = plt.subplots(nrows=1, ncols=2, sharex='all', sharey='all', figsize=(20, 10), num=title, dpi=60)
plt.subplots_adjust(left=0, right=1, bottom=0, top=1, wspace=0)

edge_label_opts = {
    'font_family': 'serif',
    'font_color': 'black',
    'font_size': 10,
    'font_weight': 'normal',
    'horizontalalignment': 'left',
    'bbox': {'facecolor': 'white', 'edgecolor': 'none', 'pad': 0.3, 'alpha': 1},
}

pos = nx.shell_layout(G1 )
#arr_pos = np.array(list(pos.values()), dtype='object')
#print(arr_pos)
#pos = nx.rescale_layout(arr_pos, 5)
if init_graph == 'Test_graph.txt':
    pos = {0: (-2, 0), 1: (-1, 0.5), 2: (0, 0.5), 3: (1, 0.5), 4: (2, 0), 5: (1, -0.5), 6: (0, -0.5), 7: (-1, -0.5), 8: (0, 0)}

nx.draw(G1, pos, with_labels=True, node_size=150, ax=ax[0])
labels1 = nx.get_edge_attributes(G1, "weight")
nx.draw_networkx_edge_labels(G1, pos, edge_labels=labels1, **edge_label_opts, ax=ax[0])


nx.draw(G2, pos, with_labels=True, node_size=150, ax=ax[1])
labels2 = nx.get_edge_attributes(G2, "weight")
nx.draw_networkx_edge_labels(G2, pos, edge_labels=labels2, **edge_label_opts, ax=ax[1])

plt.show()
