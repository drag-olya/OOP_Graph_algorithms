#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <fstream>

using namespace std;
static vector<int> DEFAULT_VECTOR;

class Graph {

private:

    void DFS_recursion(int u, vector<int>& visited, vector<int>& parent) {

        visited[u] = 1;

        vector<pair<int, int>> adj_list = edges[u];
        for (auto adj_node : adj_list) {
            if (!visited[adj_node.first]) {

                parent[adj_node.first] = u;

                DFS_recursion(adj_node.first, visited, parent);
            }
        }

    }

    pair<int, int> min_wgh_edge(vector<int> key, vector<int> visited) {

        int min_wgh = INT_MAX, min_index = -1;

        for (int v = 0; v < n; v++) {
            if (!visited[v] && key[v] < min_wgh)
            {
                min_wgh = key[v], min_index = v;
            }

        }

        return make_pair(min_index, min_wgh);

    }

    void print_path(vector<int> parent, ofstream& f) {

        for (int u = 0; u < n; u++) {
            int par_u = parent[u];
            if (par_u >= 0) {
                f << par_u << " " << u << " " << getWgh(par_u, u) << endl;
            }
            
        }
    }

    void print_Dijkstra(vector<int> parent) {

        ofstream f("Dijkstra.txt");

        for (int i = 0; i < n; i++)
        {
            int par_i = parent[i];

            if (par_i >= 0) {
                f << par_i << " " << i << " " << getWgh(par_i, i) << endl;
            }
        }
    }

    void print_shortest_path(int curr, vector<int> parent, ofstream& f) {

        stack<int> path;
        path.push(curr);

        while (parent[curr] >= 0) {
            curr = parent[curr];
            path.push(curr);
        }
        path.pop();

        while (!path.empty()) {
            curr = path.top();

            int par_curr = parent[curr];
            f << par_curr << " " << curr << " " << getWgh(par_curr, curr) << endl;

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

        ofstream f("FordFulkerson.txt");

        for (int u = 0; u < n; u++)
        {
            for (pair<int, int> adj_node : edges[u]) {

                int v = adj_node.first;
                int g_wgh = adj_node.second;
                int max_flow_wgh = g_wgh - resid_g.getWgh(u, v);
                if (max_flow_wgh > 0) {
                    f << u << " " << v << " " << max_flow_wgh << endl;
                }

            }
        }

    }

    int search_step(vector<int> dist, vector<int>& visited) {

        pair<int, int> min_u = min_wgh_edge(dist, visited);
        int u = min_u.first;

        visited[u] = 1;

        return u;
    }

    void upd_shortest_dist(int w, int x, int dist_w_x, vector<int> visited, vector<int>& dist, vector<int>& parent) {

        if (!visited[x]) {

            int dist_x = dist[w] + dist_w_x;

            if (dist_x < dist[x]) {
                dist[x] = dist_x;
                parent[x] = w;
            }
        }
    }

    void upd_mu(vector<int> visited, vector<int> dist_f, vector<int> dist_b, int x, int& mu, int& meet) {
        
        if (visited[x]) {
            int dist_start_goal = dist_f[x] + dist_b[x];

            if (dist_start_goal < mu) {
                mu = dist_start_goal;
                meet = x;
            }
        }
    }

    void print_bidirect(int meet, vector<int> parent_f, vector<int> parent_b, vector<int> dist_f, string fname) {

        ofstream f(fname);
        print_shortest_path(meet, parent_f, f);
        ofstream f_app(fname, std::ios_base::app);
        print_shortest_path(meet, parent_b, f_app);

    }
  
    int get_k(vector<int> visited, vector<int> f_score) {

        int k_min = INT_MAX;
        for (int i = 0; i < n; i++)
        {
            if (!visited[i] && f_score[i] < k_min) {
                k_min = f_score[i];
            }
        }
        return k_min;
    }

public:

    vector<vector<pair<int, int>>> edges;
    vector<pair<int, int>> coordinates;
    int n;
    
    Graph(int size) {
        edges.resize(size);
        coordinates.resize(size);
        n = size;
    }

    void addEdge(int start, int end, int weight) {

        edges[start].push_back(make_pair(end, weight));
    }

    void addCoord(int v, int x, int y) {

        coordinates[v] = make_pair(x, y);
    }

    int getWgh(int u, int v) {

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

    int BFS(int start, int goal = -1, vector<int>& parent_for_FF = DEFAULT_VECTOR) {

        vector<int> visited(n, 0), parent(n, -1);
        queue<int> q;
        bool FordFulkers_method = false;

        if (!parent_for_FF.empty()) {
            FordFulkers_method = true;
        }

        q.push(start);
        visited[start] = 1;

        if (FordFulkers_method) { parent_for_FF[start] = -1; }
        else { parent[start] = -1; }

        while (! q.empty()) {
            int v = q.front();
            q.pop();

            vector<pair<int, int>> adj_list = edges[v];

            for (auto adj_node: adj_list) {

                if (! visited[adj_node.first] && adj_node.second > 0) {

                    q.push(adj_node.first);

                    if (FordFulkers_method) { parent_for_FF[adj_node.first] = v; }
                    else { parent[adj_node.first] = v; }
                    
                    visited[adj_node.first] = 1;
                }
            }
        }

        if (!FordFulkers_method) { 
            ofstream f("BFS.txt"); 
            print_path(parent, f); 
        }

        if (goal >= 0) {
            return visited[goal];
        }
        return 0;
    }

    void DFS(int start) {

        vector<int> visited(n, 0), parent(n, -1);
        parent[start] = -1;

        DFS_recursion(start, visited, parent);

        ofstream f("DFS.txt");
        print_path(parent, f);
    }

    void Prim(int u) {

        vector<int> dist(n, INT_MAX), visited(n, 0), parent(n, -1);

        ofstream f("PRIM.txt");

        dist[u] = 0;
        visited[u] = 1;

        for (int i = 0; i < n - 1;i++){

            for (pair<int, int> adj_node : edges[u]) {
                int v_adj = adj_node.first;

                if (!visited[v_adj] && adj_node.second < dist[v_adj]) {

                    parent[v_adj] = u;
                    dist[v_adj] = adj_node.second;
                }
            }
            
            pair<int, int> min_v = min_wgh_edge(dist, visited);
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
                int dist_curr_v = adj_node.second;

                upd_shortest_dist(curr, v, dist_curr_v, visited, dist, parent);
            }
        }

        print_Dijkstra(parent);
    }

    int h(int u, int v) {
        
        int x1 = coordinates[u].first;
        int y1 = coordinates[u].second;
        int x2 = coordinates[v].first;
        int y2 = coordinates[v].second;

        return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    }

    void AStar(int start, int goal) {

        vector<int> visited(n, 0), g_score(n, INT_MAX), f_score(n, INT_MAX), parent(n, -1); // f(n) = g(n) + h(n)

        g_score[start] = 0;
        f_score[start] = h(start, goal);
        int curr = start;

        for (int count = 0; count < n - 1; count++) {

            pair<int, int> curr_node = min_wgh_edge(f_score, visited);
            curr = curr_node.first;

            if (curr == goal) {
                ofstream f("AStar.txt");
                return print_shortest_path(curr, parent, f);
            }
            visited[curr] = 1;

            for (pair<int, int> adj_node : edges[curr]) {

                int v = adj_node.first;
                int d = adj_node.second;

                int dist_v = g_score[curr] + d;
                if (dist_v < g_score[v]) {
                    parent[v] = curr;
                    g_score[v] = dist_v;
                    f_score[v] = dist_v + h(v, goal);
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
                path_flow = min(path_flow, resid_g.getWgh(u, v));
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

    void BidirectDijkstra(int start, int goal) {

        vector<int> visited_f(n, 0), dist_f(n, INT_MAX), parent_f(n, -1); // f - forward approximations;
        vector<int> visited_b(n, 0), dist_b(n, INT_MAX), parent_b(n, -1); // b - backward approximations
        int meet;
        int mu = INT_MAX;  //best s - g path seen so far.


        dist_f[start] = 0; dist_b[goal] = 0;

        for (int count = 0; count < n - 1; count++) {

            int u = search_step(dist_f, visited_f);
            int v = search_step(dist_b, visited_b);

            for (pair<int, int> adj_node : edges[u]) {
                
                int x = adj_node.first;
                int dist_u_x = adj_node.second;

                upd_shortest_dist(u, x, dist_u_x, visited_f, dist_f, parent_f);

                upd_mu(visited_b, dist_f, dist_b, x, mu, meet);

            }

            for (pair<int, int> adj_node : edges[v]) {

                int x = adj_node.first;
                int dist_v_x = adj_node.second;

                upd_shortest_dist(v, x, dist_v_x, visited_b, dist_b, parent_b);

                upd_mu(visited_f, dist_b, dist_f, x, mu, meet);

            }

            if (dist_f[u] + dist_b[v] >= mu && u != start && v != goal) {
                break;
            }

        }

        print_bidirect(meet, parent_f, parent_b, dist_f, "BidirectDijkstra.txt");
    }

    void BidirectAStar(int start, int goal) {

        vector<int> visited_f(n, 0), g_score_f(n, INT_MAX), f_score_f(n, INT_MAX),  parent_f(n, -1);
        vector<int> visited_b(n, 0), dist_b(n, INT_MAX), g_score_b(n, INT_MAX), f_score_b(n, INT_MAX), parent_b(n, -1);
   
        int meet;
        int mu = INT_MAX;  //best s - g path seen so far.

        g_score_f[start] = 0; g_score_b[goal] = 0;
        f_score_f[start] = h(start, goal); f_score_b[goal] = h(goal, start); //////?

        for (int count = 0; count < n - 1; count++) {

            int u = search_step(f_score_f, visited_f);
            int v = search_step(f_score_b, visited_b);

            for (pair<int, int> adj_node : edges[u]) {

                int x = adj_node.first;
                int dist_u_x = adj_node.second;

                if (!visited_f[x]) {
                    int dist_x = g_score_f[u] + dist_u_x;

                    if (dist_x < g_score_f[x]) {
                        parent_f[x] = u;
                        g_score_f[x] = dist_x;
                        f_score_f[x] = dist_x + h(x, goal);
                    }
                }
                upd_mu(visited_b, g_score_f, g_score_b, x, mu, meet);
            }

            for (pair<int, int> adj_node : edges[v]) {

                int x = adj_node.first;
                int dist_v_x = adj_node.second;

                if (!visited_b[x]) {
                    int dist_x = g_score_b[v] + dist_v_x;

                    if (dist_x < g_score_b[x]) {
                        parent_b[x] = v;
                        g_score_b[x] = dist_x;
                        f_score_b[x] = dist_x + h(x, start);
                    }
                }
                upd_mu(visited_f, g_score_b, g_score_f, x, mu, meet);
            }

            int k_f = get_k(visited_f, f_score_f);
            int k_b = get_k(visited_b, f_score_b);

            if (max(k_f, k_b) >= mu && u != start && v != goal) {
                break;
            }
        }
        print_bidirect(meet, parent_f, parent_b, g_score_f, "BidirectAStar.txt");
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

void write_graph_to_file(Graph g, string fname)
{
    ofstream f(fname);

    for (int i = 0; i < g.n; i++)
    {
        for (pair<int, int> p : g.edges[i]) {
            int w = p.second;

            if (w) {
                f << i << " " << p.first << " " << p.second << endl;
            }
            
        }
    }
}

void visualize(const char* init_graph, const char* res_graph) {

    FILE *fp;
    int status;
    const int MAX_SIZE = 256;
    char input[256];

    strcpy_s(input, init_graph);
    strcat_s(input, " ");
    strcat_s(input, res_graph);


    fp = _popen("python Graph_visualization.py", "w");
    
    if (fp == NULL) {
        cout << "Failed to open pipe!" << endl;
    }
    else {
        fputs(input, fp);
    }

    status = _pclose(fp);

    if (status == -1) {
        cout << "Smth went wrong!" << endl;
    }
    else {
    }
}

Graph rand_weighted_g(int n) {

    Graph rand_g(n);
    srand(time(0));

    for (int i = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            
            if (i != j) {
                int connect = rand() % 2;
                if (connect) {
                    int w = (float)rand() / 1000;
                    rand_g.addEdge(i, j, w);
                    rand_g.addEdge(j, i, w);
                }
                
            }
            
        }
    }

    return rand_g;
}

int main()
{

    Graph rand_g = rand_weighted_g(10);
    write_graph_to_file(rand_g, "Rand_g.txt");
    rand_g.print();

    rand_g.DFS(3);
    rand_g.Prim(3);
    visualize("Rand_g.txt", "Prim.txt");

    Graph test_graph = read_graph("Test_graph.txt");
    test_graph.addCoord(0, -2, 0);
    test_graph.addCoord(1, -1, 0.5);
    test_graph.addCoord(2, 0, 0.5);
    test_graph.addCoord(3, 1, 0.5);
    test_graph.addCoord(4, 2, 0);
    test_graph.addCoord(5, 1, -0.5);
    test_graph.addCoord(6, 0, -0.5);
    test_graph.addCoord(7, -1, -100);
    test_graph.addCoord(8, 0, 0);

    Graph test_graph_FF = read_graph("Test_graph_FF.txt");

    test_graph.BFS(2);
    test_graph.DFS(2);
    test_graph.Prim(0);
    test_graph.Dijkstra(0, 5);
    test_graph.AStar(0, 5);
    test_graph_FF.FordFulkerson(0, 5);
    test_graph.BidirectDijkstra(0, 5);
    test_graph.BidirectAStar(0, 5);

    //visualize("Test_graph.txt", "BidirectDijkstra.txt");
    //visualize("Test_graph.txt", "BidirectAStar.txt");


    return 0;
}