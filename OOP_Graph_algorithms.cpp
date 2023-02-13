#include <iostream>
#include <vector>
#include <queue>
#include <stack>
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

    void print_Dijkstra(vector<int> parent, vector<int> dist) {

        ofstream f("Dijkstra.txt");

        for (int i = 0; i < n; i++)
        {
            if (parent[i] >= 0) {
                f << parent[i] << " " << i << " " << dist[i] << endl;
            }
        }
    }

    void print_shortest_path(int curr, vector<int> parent, ofstream& f) {

        //ofstream f("AStar.txt");

        stack<int> path;
        path.push(curr);

        while (parent[curr] >= 0) {
            curr = parent[curr];
            path.push(curr);
        }

        while (!path.empty()) {
            f << path.top() << " ";
            path.pop();
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

        vector<int> weight(n, INT_MAX), visited(n, 0), parent(n, -1);

        ofstream f("PRIM.txt");

        weight[u] = 0;
        visited[u] = 1;

        for (int i = 0; i < n - 1;i++){

            for (pair<int, int> adj_node : edges[u]) {
                int v_adj = adj_node.first;

                if (!visited[v_adj] && adj_node.second < weight[v_adj]) {

                    parent[v_adj] = u;
                    weight[v_adj] = adj_node.second;
                }
            }
            
            pair<int, int> min_v = min_wgh_edge(weight, visited);
            u = min_v.first;
            visited[u] = 1;
            f << parent[u] << " " << u << " " << min_v.second << endl;
        }
    }

    void Dijkstra(int start, int goal = -1) {

        vector<int> visited(n, 0), dist(n, INT_MAX), parent(n, -1);
       
        dist[start] = 0;

        int curr = start;

        for (int count = 0; count < n - 1; count++) {

            pair<int, int> curr_node = min_wgh_edge(dist, visited);
            curr = curr_node.first;

            if (goal >= 0 && curr == goal) {
                ofstream f("Dijkstra.txt");
                return print_shortest_path(curr, parent, f);
            }

            visited[curr] = 1;

            for (pair<int, int> adj_node : edges[curr]) {

                int v = adj_node.first;

                if (!visited[v]) {

                    int dist_w = dist[curr] + adj_node.second;

                    if (dist_w < dist[v]) {
                        dist[v] = dist_w;
                        parent[v] = curr;
                    }
                }
            }

        }
        print_Dijkstra(parent, dist);
    }

    int h(int x) {
        return 1;
    }

    void AStar(int start, int goal) {

        vector<int> visited(n, 0), g(n, INT_MAX), f(n, INT_MAX), parent(n, -1); // f(n) = g(n) + h(n)

        g[start] = 0;
        f[start] = h(start);
        int curr = start;

        for (int count = 0; count < n - 1; count++) {

            pair<int, int> curr_node = min_wgh_edge(f, visited);
            curr = curr_node.first;

            if (curr == goal) {
                ofstream f("AStar.txt");
                return print_shortest_path(curr, parent, f);
            }
            visited[curr] = 1;

            for (pair<int, int> adj_node : edges[curr]) {

                int v = adj_node.first;
                int d = adj_node.second;

                int dist_v = g[curr] + d;
                if (dist_v < g[v]) {
                    parent[v] = curr;
                    g[v] = dist_v;
                    f[v] = dist_v + h(v);
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

    //g.DFS(0);

    Graph test_graph = read_graph("Adjacency Matrix.txt");

    test_graph.print(); cout << endl;
    test_graph.BFS(2);
    test_graph.DFS(2);
    test_graph.Prim(0);
    test_graph.Dijkstra(0, 7);


    return 0;
}