#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

class Graph {

public:

    vector<vector<pair<int, int>>> edges;
    int n;

    Graph(int size) {
        edges.resize(size);
        n = size;
    }

    void addEdge(int start, int end, int weight) {

        edges[start].push_back(make_pair(end, weight));
    }

    void print() {

        for (int i = 0; i < n; i++)
        {
            for (auto p : edges[i]) {
                cout << "edge (" << i << ", " << p.first << "), weight: " << p.second << endl;
            }
        }
        
    }

    void BFS(int u) {

        vector<int> visited(n, 0);
        queue<int> q;

        q.push(u);
        visited[u] = 1;

        ofstream f("BFS.txt");

        while (! q.empty()) {
            int v = q.front();
            f << v << " ";
            cout << v << " ";
            q.pop();

            vector<pair<int, int>> adj_list = edges[v];
            for (auto adj_node: adj_list) {
                if (! visited[adj_node.first]) {
                    q.push(adj_node.first);
                    visited[adj_node.first] = 1;
                }
                
            }
        }


    }

};


Graph read_graph(string fname) {

    int n, x;

    ifstream f(fname);

    f >> n;
    Graph g(n);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++) {
            f >> x;
            if (x) {
                g.addEdge(i, j, x);
            }
        }
    }
    return g;
}

int main()
{
    Graph g(3);

    g.addEdge(1, 2, 10);
    g.addEdge(1, 0, 10);
    g.addEdge(2, 1, 10);
    g.addEdge(2, 0, 10);
    g.addEdge(0, 2, 10);
    g.addEdge(0, 1, 10);

    g.BFS(0);

    Graph g_f = read_graph("Adjacency Matrix.txt");

    g_f.print();


    return 0;
}