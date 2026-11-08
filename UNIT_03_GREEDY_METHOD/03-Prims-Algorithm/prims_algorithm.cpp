/*
 * Program : Prim's Algorithm - Minimum Cost Spanning Tree (Greedy)
 * Unit    : 3 - Greedy Method
 *
 * Greedy choice : tree is linked with every vertexd, and further vertex outsdie the tree is linked with edge 
 *                 outside the vertex,with the lowest cost.
 * Representation: Adjacency Matrix
 * Time          : O(V^2)
 * Space         : O(V^2) matrix
 * Vertices 0-based: 0 se V-1
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
using namespace std;

const int INF = numeric_limits<int>::max();

struct MSTEdge {
    int u, v, weight;
};

int readInt(const string &prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Integer dalo: ";
    }
    return value;
}

int readIntInRange(const string &prompt, int minVal, int maxVal) {
    int v = readInt(prompt);
    while (v < minVal || v > maxVal)
        v = readInt("Invalid! " + to_string(minVal) + " se " + to_string(maxVal) + " ke beech dalo: ");
    return v;
}

// Return: true , if graph connected.
bool primMST(const vector<vector<int>> &graph, int V, vector<MSTEdge> &mst, long long &totalCost) {
    vector<int> key(V, INF);          // tree to vertex,, smallest edge.
    vector<int> parent(V, -1);
    vector<bool> inMST(V, false);
    key[0] = 0;                       // vertex 0 
    totalCost = 0;
    mst.clear();

    for (int count = 0; count < V; count++) {
        
        int u = -1;
        for (int v = 0; v < V; v++)
            if (!inMST[v] && (u == -1 || key[v] < key[u])) u = v;

        if (key[u] == INF) return false;       // no vertex reachable-> disconnected

        inMST[u] = true;
        if (parent[u] != -1) {
            mst.push_back({parent[u], u, key[u]});
            totalCost += key[u];
        }
        
        for (int v = 0; v < V; v++) {
            if (graph[u][v] != INF && !inMST[v] && graph[u][v] < key[v]) {
                key[v] = graph[u][v];
                parent[v] = u;
            }
        }
    }
    return true;
}

int main() {
    int V = readIntInRange("Vertices ki sankhya (1 se 2000): ", 1, 2000);
    long long maxEdges = (long long)V * (V - 1) / 2;
    int E = readIntInRange("Edges ki sankhya (0 se " + to_string(maxEdges) + "): ", 0, (int)min<long long>(maxEdges, 2000000));

    vector<vector<int>> graph(V, vector<int>(V, INF));
    cout << "Har edge: u v weight  (vertices 0 se " << V - 1 << ", weight >= 0)\n";
    for (int i = 0; i < E; i++) {
        int u = readInt("Edge " + to_string(i + 1) + " u: ");
        int v = readInt("Edge " + to_string(i + 1) + " v: ");
        int w = readInt("Edge " + to_string(i + 1) + " weight: ");
        if (u < 0 || u >= V || v < 0 || v >= V || u == v || w < 0) {
            cout << "Invalid edge! Dobara dalo.\n";
            i--;
            continue;
        }
        if (w < graph[u][v]) graph[u][v] = graph[v][u] = w;   
    }

    vector<MSTEdge> mst;
    long long total;
    if (!primMST(graph, V, mst, total)) {
        cout << "\nGraph connected nahi hai, isliye spanning tree nahi ban sakta.\n";
        return 1;
    }

    cout << "\nMST edges (Prim):\n";
    for (const MSTEdge &e : mst) cout << "  " << e.u << " - " << e.v << "  (weight " << e.weight << ")\n";
    cout << "Total MST cost = " << total << "\n";
    return 0;
}
