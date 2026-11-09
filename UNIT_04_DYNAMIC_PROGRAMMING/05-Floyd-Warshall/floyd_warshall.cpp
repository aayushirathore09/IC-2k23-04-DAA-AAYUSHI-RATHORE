/*
 * Program : Floyd-Warshall - All-Pairs Shortest Path (Dynamic Programming)
 * Unit    : 4 - Dynamic Programming
 *
 * dist[i][j] ko baar-baar improve karte hain: kya vertex k ke through jaana sasta hai?
 *   dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])   har k = 0..V-1 ke liye
 *
 * Time    : O(V^3)
 * Space   : O(V^2)
 * Notes   : negative edge weights chalte hain, par negative cycle nahi (program detect karta hai)
 *           Vertices 0-based: 0 se V-1
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
using namespace std;

const long long INF = numeric_limits<long long>::max() / 4;

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

// dist aur next (path rebuild ke liye) fill karta hai.
// Return: false agar negative cycle mili
bool floydWarshall(vector<vector<long long>> &dist, vector<vector<int>> &next, int V) {
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            if (dist[i][k] == INF) continue;            // INF + INF overflow se bachne ke liye
            for (int j = 0; j < V; j++) {
                if (dist[k][j] == INF) continue;
                if (dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
    for (int i = 0; i < V; i++)
        if (dist[i][i] < 0) return false;              // negative cycle
    return true;
}

// u se v tak ka actual path
vector<int> buildPath(const vector<vector<int>> &next, int u, int v) {
    vector<int> path;
    if (next[u][v] == -1) return path;
    path.push_back(u);
    while (u != v) {
        u = next[u][v];
        path.push_back(u);
    }
    return path;
}

void printMatrix(const vector<vector<long long>> &dist, int V) {
    cout << "     ";
    for (int j = 0; j < V; j++) cout << setw(6) << j;
    cout << "\n";
    for (int i = 0; i < V; i++) {
        cout << setw(4) << i << " ";
        for (int j = 0; j < V; j++) {
            if (dist[i][j] == INF) cout << setw(6) << "INF";
            else                   cout << setw(6) << dist[i][j];
        }
        cout << "\n";
    }
}

int main() {
    int V = readIntInRange("Vertices ki sankhya (1 se 200): ", 1, 200);
    int type = readIntInRange("Graph type: 1 = Directed, 2 = Undirected: ", 1, 2);
    long long maxEdges = (long long)V * (V - 1) / (type == 1 ? 1 : 2);   // directed: V(V-1), undirected: V(V-1)/2
    int E = readIntInRange("Edges ki sankhya (0 se " + to_string(maxEdges) + "): ", 0, (int)maxEdges);

    vector<vector<long long>> dist(V, vector<long long>(V, INF));
    vector<vector<int>> next(V, vector<int>(V, -1));
    for (int i = 0; i < V; i++) { dist[i][i] = 0; next[i][i] = i; }

    cout << "Har edge: u v weight  (vertices 0 se " << V - 1 << ")\n";
    for (int i = 0; i < E; i++) {
        int u = readInt("Edge " + to_string(i + 1) + " u: ");
        int v = readInt("Edge " + to_string(i + 1) + " v: ");
        int w = readInt("Edge " + to_string(i + 1) + " weight: ");
        if (u < 0 || u >= V || v < 0 || v >= V || u == v) {
            cout << "Invalid edge! Dobara dalo.\n";
            i--;
            continue;
        }
        if (type == 2 && w < 0) {
            cout << "Undirected graph me negative weight negative cycle bana deta hai. Dobara dalo.\n";
            i--;
            continue;
        }
        if (w < dist[u][v]) {                         // duplicate edge me sasta rakho
            dist[u][v] = w; next[u][v] = v;
            if (type == 2) { dist[v][u] = w; next[v][u] = u; }
        }
    }

    if (!floydWarshall(dist, next, V)) {
        cout << "\nNegative cycle mili! Shortest paths defined nahi hain.\n";
        return 1;
    }

    cout << "\nAll-pairs shortest distance matrix:\n";
    printMatrix(dist, V);

    char again;
    cout << "\nKisi do vertices ka path dekhna hai? (y/n): ";
    cin >> again;
    while (again == 'y' || again == 'Y') {
        int u = readIntInRange("Source: ", 0, V - 1);
        int v = readIntInRange("Destination: ", 0, V - 1);
        vector<int> path = buildPath(next, u, v);
        if (path.empty()) {
            cout << "Vertex " << u << " se " << v << " tak koi path nahi hai.\n";
        } else {
            cout << "Shortest distance = " << dist[u][v] << "\nPath: ";
            for (size_t i = 0; i < path.size(); i++) cout << path[i] << (i + 1 < path.size() ? " -> " : "\n");
        }
        cout << "Aur ek path? (y/n): ";
        cin >> again;
    }
    return 0;
}
