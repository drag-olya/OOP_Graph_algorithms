#include <iostream>
#include <vector>
#include <queue>
#include <fstream>

using namespace std;

class Graph {

private:

    void DFS_recursion(int u, vector<int>& visited, ofstream& f) {

        visited[u] = 1;
        f << u << " ";

        vector<pair<int, int>> adj_list = edges[u];
        for (auto adj_node : adj_list) {
            if (!visited[adj_node.first]) {
                DFS_recursion(adj_node.first, visited, f);
            }
        }

    }

    pair<int, int> min_wgh_edge(vector<int> key, vector<int> visited) {

        int min_wgh = INT_MAX, min_index;

        for (int v = 0; v < n; v++) {

            if (!visited[v] && key[v] < min_wgh)
                min_wgh = key[v], min_index = v;
        }

        return make_pair(min_index, min_wgh);
    }

    void _print_q(vector<pair<int, int>> q) { ////////////////////////////////////////////////////////delete
        for (pair<int, int> adj_node : q) {
            cout << adj_node.first << " " << adj_node.second << endl;
        }
    }

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

    void DFS(int u) {

        vector<int> visited(n, 0);
        ofstream f("DFS.txt");

        DFS_recursion(u, visited, f);
    }


    

    void Prim(int u) {

        vector<int> key(n, INT_MAX), visited(n, 0);
        vector<int> parent(n, -1);

        ofstream f("PRIM.txt");

        key[u] = 0;
        visited[u] = 1;

        for (int i = 0; i < n - 1;i++){

            for (pair<int, int> adj_node : edges[u]) {
                int v_adj = adj_node.first;

                if (!visited[v_adj] && adj_node.second < key[v_adj]) {
                    parent[v_adj] = u;
                    key[v_adj] = adj_node.second;
                }
            }
            
            pair<int, int> min_v = min_wgh_edge(key, visited);
            u = min_v.first;
            visited[u] = 1;
            f << parent[u] << " " << u << " " << min_v.second << endl;
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

    //g.DFS(0);

    Graph g_f = read_graph("Adjacency Matrix.txt");

    g_f.print(); cout << endl;
    g_f.BFS(2);
    g_f.DFS(2);
    g_f.Prim(0);


    return 0;
}