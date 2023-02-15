#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>

using namespace std;
static vector<int> DEFAULT_VECTOR;

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

    void print_shortest_path(int curr, vector<int> parent, vector<int> dist, ofstream& f) {

        stack<int> path;
        path.push(curr);

        while (parent[curr] >= 0) {
            curr = parent[curr];
            path.push(curr);
        }
        path.pop();

        while (!path.empty()) {
            curr = path.top();

            f << parent[curr] << " " << curr << " " << dist[curr] << endl;

            path.pop();
            
        }
    }

    void upd_wgh(int u, int v, int new_wgh) {

        for (pair<int, int>& adj_node : edges[u]) {

            auto* p_wgh = &adj_node.second;

            if (adj_node.first == v) {
                *p_wgh += new_wgh;
            }
        }
    }

    void print_FordFulkerson(Graph resid_g) {

        ofstream f("Ford Fulkerson.txt");

        for (int u = 0; u < n; u++)
        {
            for (pair<int, int> adj_node : edges[u]) {

                int v = adj_node.first;
                int g_wgh = adj_node.second;
                int max_flow_wgh = g_wgh - resid_g.get_wgh(u, v);
                if (max_flow_wgh > 0) {
                    f << u << " " << v << " " << max_flow_wgh << endl;
                }

            }
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

    int get_wgh(int u, int v) {

        for (pair<int, int> adj_node : edges[u]) {

            if (adj_node.first == v) {
                return adj_node.second;
            }
        }
        return 0;
    }

    void print() {

        for (int i = 0; i < n; i++)
        {
            for (pair<int, int> p : edges[i]) {
                cout << "edge (" << i << ", " << p.first << "), weight: " << p.second << endl;
            }
        }
        
    }

    int BFS(int start, int goal = -1, vector<int>& parent = DEFAULT_VECTOR) {

        vector<int> visited(n, 0);
        queue<int> q;

        q.push(start);
        visited[start] = 1;

        if (!parent.empty()) {
            parent[start] = -1;
        }
        //

        ofstream f("BFS.txt");

        while (! q.empty()) {
            int v = q.front();
            f << v << " ";
            q.pop();

            vector<pair<int, int>> adj_list = edges[v];

            for (auto adj_node: adj_list) {

                if (! visited[adj_node.first] && adj_node.second > 0) {

                    q.push(adj_node.first);

                    if (!parent.empty()) { parent[adj_node.first] = v; }
                    
                    visited[adj_node.first] = 1;
                }
            }
        }
        if (goal >= 0) {
            return visited[goal];
        }
        return 0;
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
                return print_shortest_path(curr, parent, dist, f);
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

        vector<int> visited(n, 0), g_score(n, INT_MAX), f_score(n, INT_MAX), parent(n, -1); // f(n) = g(n) + h(n)

        g_score[start] = 0;
        f_score[start] = h(start);
        int curr = start;

        for (int count = 0; count < n - 1; count++) {

            pair<int, int> curr_node = min_wgh_edge(f_score, visited);
            curr = curr_node.first;

            if (curr == goal) {
                ofstream f("AStar.txt");
                return print_shortest_path(curr, parent, f_score, f);
            }
            visited[curr] = 1;

            for (pair<int, int> adj_node : edges[curr]) {

                int v = adj_node.first;
                int d = adj_node.second;

                int dist_v = g_score[curr] + d;
                if (dist_v < g_score[v]) {
                    parent[v] = curr;
                    g_score[v] = dist_v;
                    f_score[v] = dist_v + h(v);
                }
            }

        }

    }

    void FordFulkerson(int start, int goal) {

        Graph resid_g(n);
        resid_g.edges = edges;

        vector<int> parent(n, -1);
        int u, max_flow = 0;
        
        while (resid_g.BFS(start, goal, parent)) {
       
            int path_flow = INT_MAX;

            for (int v = goal; v != start; v = parent[v]) {
                
                u = parent[v];
                path_flow = min(path_flow, resid_g.get_wgh(u, v));
            }
            for (int v = goal; v != start; v = parent[v]) {

                u = parent[v];
                resid_g.upd_wgh(u, v, -path_flow);
                resid_g.upd_wgh(v, u, path_flow);
            }

            max_flow += path_flow;
        }

        print_FordFulkerson(resid_g);

        cout << "Max flow: " << max_flow << endl;
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

    Graph test_graph = read_graph("Test graph.txt");

    //test_graph.print(); cout << endl;
    test_graph.BFS(2);
    test_graph.DFS(2);
    test_graph.Prim(0);
    test_graph.Dijkstra(0);
    test_graph.AStar(0, 6);

    Graph test_graph_FF = read_graph("Test graph FF.txt");
    test_graph_FF.print(); cout << endl;
    test_graph_FF.FordFulkerson(0, 5);

    //cout << test_graph.FordFulkerson(0, 5) << endl;


    return 0;
}