import matplotlib.pyplot as plt
import networkx as nx

files = input()

init_graph, res_graph = files.split()[0], files.split()[1]

#init_graph = r"Test_graph.txt"
with open(init_graph, "r") as f1:
    lines1 = f1.readlines()
print('geeee')
# Create an empty graph for the second filemain.py
G1 = nx.Graph()

for i, line in enumerate(lines1[1:]):
    data = line.split()
    for j, weight in enumerate(data):
        if float(weight) > 0:
            G1.add_edge(i, j, weight=float(weight))



# Read in the file
#res_graph = "BidirectAStar.txt"
with open(res_graph, "r") as f2:
    lines2 = f2.readlines()

# Create an empty graph
G2 = nx.Graph()

# Add nodes and edges to the graph
for line in lines2:
    data = line.split()
    node1 = int(data[0])
    node2 = int(data[1])
    weight = float(data[2])
    G2.add_edge(node1, node2, weight=weight)

fig, (ax1, ax2) = plt.subplots(ncols=2, figsize=(13, 8))


edge_label_opts = {
    'font_family': 'serif',
    'font_color': 'black',
    'font_size': 8,
    'font_weight': 'normal',
    'bbox': {'facecolor': 'white', 'edgecolor': 'none', 'pad': 0.3, 'alpha': 1},
}

# Draw the graph
pos1 = nx.shell_layout(G1)
print(pos1)
nx.draw(G1, pos1, with_labels=True, ax=ax1)
labels1 = nx.get_edge_attributes(G1, "weight")
label_pos = {k: (v[0], v[1]+0.1) for k, v in pos1.items()}
nx.draw_networkx_edge_labels(G1, pos1, edge_labels=labels1, **edge_label_opts, ax=ax1)


nx.draw(G2, pos1, with_labels=True, ax=ax2)
labels2 = nx.get_edge_attributes(G2, "weight")
nx.draw_networkx_edge_labels(G2, pos1, edge_labels=labels2, **edge_label_opts, ax=ax2)

plt.show()