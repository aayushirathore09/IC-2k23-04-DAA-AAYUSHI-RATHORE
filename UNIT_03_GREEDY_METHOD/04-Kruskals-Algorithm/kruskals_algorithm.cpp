/*
 * Program : Kruskal's Algorithm - Minimum Cost Spanning Tree (Greedy)
 * Unit    : 3 - Greedy Method
 *
 * Greedy choice : sort in ascending order, acc to weight.
 *                take the smallest edge.
 * Representation: Edge list
 * Time          : O(E log E) = O(E log V)
 * Space         : O(V + E)
 * Vertices 0-based: 0 se V-1
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <numeric>
using namespace std;

struct Edge {
    int u, v, weight;
};

// Disjoint Set Union (Union-Find): path compression + union by rank
class DSU {
    vector<int> parent, rnk;
public:
    explicit DSU(int n) : parent(n), rnk(n, 0) { iota(parent.begin(), parent.end(), 0); }

    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);   // path compression
        return parent[x];
    }
    // Return false if cycle form,
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rnk[a] < rnk[b]) swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b]) rnk[a]++;
        return true;
    }
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

// Return: true if mst made 
bool kruskalMST(vector<Edge> edges, int V, vector<Edge> &mst, long long &totalCost) {
    sort(edges.begin(), edges.end(),
         [](const Edge &a, const Edge &b) { return a.weight < b.weight; });

    DSU dsu(V);
    mst.clear();
    totalCost = 0;

    for (const Edge &e : edges) {
        if (dsu.unite(e.u, e.v)) {            // cycle nahi bani -> edge le lo
            mst.push_back(e);
            totalCost += e.weight;
            if ((int)mst.size() == V - 1) break;   // MST complete
        }
    }
    return (int)mst.size() == V - 1;
}

int main() {
    int V = readIntInRange("Vertices ki sankhya (1 se 2000): ", 1, 2000);
    long long maxEdges = (long long)V * (V - 1) / 2;
    int E = readIntInRange("Edges ki sankhya (0 se " + to_string(maxEdges) + "): ", 0, (int)min<long long>(maxEdges, 2000000));

    vector<Edge> edges;
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
        edges.push_back({u, v, w});
    }

    vector<Edge> mst;
    long long total;
    if (!kruskalMST(edges, V, mst, total)) {
        cout << "\nGraph connected nahi hai, isliye spanning tree nahi ban sakta.\n";
        return 1;
    }

    cout << "\nMST edges (Kruskal):\n";
    for (const Edge &e : mst) cout << "  " << e.u << " - " << e.v << "  (weight " << e.weight << ")\n";
    cout << "Total MST cost = " << total << "\n";
    return 0;
}
