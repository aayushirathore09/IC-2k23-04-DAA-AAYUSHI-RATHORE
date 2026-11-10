/*
 * File : tsp.hpp
 * Unit : 7 - NP-Hard / NP-Complete
 * Kaam : TSP ke 4 solvers: Exhaustive (brute force), Dynamic Programming (Held-Karp),
 *        Branch & Bound (reduced cost matrix), aur Nearest Neighbour heuristic
 * Rule : complete graph, cost[i][j] >= 0 (i != j). Asymmetric cost bhi chalega.
 *        Tour city 0 se shuru hoke 0 pe hi khatam hota hai.
 */
#pragma once
#include <algorithm>
#include <climits>
#include <queue>
#include <vector>
using namespace std;

const int TSP_INF = 1000000000;      // "edge nahi hai / use nahi kar sakte"

struct TspResult {
    long long bestCost = LLONG_MAX;
    vector<int> tour;              // 0 -> ... -> 0
    long long nodesExplored = 0;   // permutations / DP states / B&B nodes
    long long memoryBytes = 0;     // data structure ki estimated peak memory
};

inline long long tourCost(const vector<vector<int>> &cost, const vector<int> &tour) {
    long long total = 0;
    for (size_t i = 0; i + 1 < tour.size(); i++) total += cost[tour[i]][tour[i + 1]];
    return total;
}

// ---------------------------------------------------------------- Exhaustive
// City 0 fix karke baaki (n-1)! permutations try karo.  n <= 11 recommended.
inline TspResult tspExhaustive(const vector<vector<int>> &cost) {
    int n = (int)cost.size();
    TspResult res;
    vector<int> perm;
    for (int i = 1; i < n; i++) perm.push_back(i);
    do {
        long long total = cost[0][perm[0]];
        for (int i = 0; i + 1 < (int)perm.size(); i++) total += cost[perm[i]][perm[i + 1]];
        total += cost[perm.back()][0];
        res.nodesExplored++;
        if (total < res.bestCost) {
            res.bestCost = total;
            res.tour.assign(1, 0);
            res.tour.insert(res.tour.end(), perm.begin(), perm.end());
            res.tour.push_back(0);
        }
    } while (next_permutation(perm.begin(), perm.end()));
    res.memoryBytes = (long long)n * sizeof(int);
    return res;
}

// ---------------------------------------------------------------- Dynamic Programming (Held-Karp)
// dp[mask][j] = 
// (mask me city 1..n-1 ke bits; bit k <-> city k+1).  Time O(n^2 2^n), Space O(n 2^n)
inline TspResult tspDP(const vector<vector<int>> &cost) {
    int n = (int)cost.size();
    TspResult res;
    if (n == 1) { res.bestCost = 0; res.tour = {0, 0}; res.memoryBytes = sizeof(int); return res; }
    int m = n - 1;                                   // 0 ke alawa cities
    size_t full = (size_t)1 << m;
    vector<int> dp(full * m, TSP_INF);
    vector<signed char> parent(full * m, -1);
    for (int j = 0; j < m; j++) dp[((size_t)1 << j) * m + j] = cost[0][j + 1];

    for (size_t mask = 1; mask < full; mask++) {
        for (int j = 0; j < m; j++) {
            if (!(mask & ((size_t)1 << j))) continue;
            int cur = dp[mask * m + j];
            if (cur >= TSP_INF) continue;
            for (int k = 0; k < m; k++) {
                if (mask & ((size_t)1 << k)) continue;
                size_t next = mask | ((size_t)1 << k);
                int cand = cur + cost[j + 1][k + 1];
                res.nodesExplored++;
                if (cand < dp[next * m + k]) { dp[next * m + k] = cand; parent[next * m + k] = (signed char)j; }
            }
        }
    }
    // Wapas city 0 pe aao
    int lastCity = -1;
    long long best = LLONG_MAX;
    for (int j = 0; j < m; j++) {
        long long total = (long long)dp[(full - 1) * m + j] + cost[j + 1][0];
        if (total < best) { best = total; lastCity = j; }
    }
    res.bestCost = best;
    // Traceback
    vector<int> rev;
    size_t mask = full - 1;
    int j = lastCity;
    while (j != -1) {
        rev.push_back(j + 1);
        int p = parent[mask * m + j];
        mask &= ~((size_t)1 << j);
        j = p;
    }
    res.tour.push_back(0);
    for (int i = (int)rev.size() - 1; i >= 0; i--) res.tour.push_back(rev[i]);
    res.tour.push_back(0);
    res.memoryBytes = (long long)full * m * (sizeof(int) + sizeof(signed char));
    return res;
}

// ---------------------------------------------------------------- Branch & Bound
// Reduced cost matrix: har row/column me se minimum .
// Best-first search (min-heap on bound).
struct TspNode {
    vector<int> matrix;     // n*n reduced matrix (flat)
    vector<int> path;       
    long long bound;        // lower bound
    int city;               
};

struct TspNodeCmp {
    bool operator()(const TspNode &a, const TspNode &b) const {
        if (a.bound != b.bound) return a.bound > b.bound;     
        return a.path.size() < b.path.size();                 
    }
};

// Matrix reduce karo, total reduction 
inline long long reduceMatrix(vector<int> &M, int n) {
    long long total = 0;
    for (int i = 0; i < n; i++) {                              // row reduction
        int mn = TSP_INF;
        for (int j = 0; j < n; j++) mn = min(mn, M[i * n + j]);
        if (mn != TSP_INF && mn > 0) {
            for (int j = 0; j < n; j++) if (M[i * n + j] != TSP_INF) M[i * n + j] -= mn;
            total += mn;
        }
    }
    for (int j = 0; j < n; j++) {                              // column reduction
        int mn = TSP_INF;
        for (int i = 0; i < n; i++) mn = min(mn, M[i * n + j]);
        if (mn != TSP_INF && mn > 0) {
            for (int i = 0; i < n; i++) if (M[i * n + j] != TSP_INF) M[i * n + j] -= mn;
            total += mn;
        }
    }
    return total;
}

inline TspResult tspNearestNeighbour(const vector<vector<int>> &cost);

inline TspResult tspBranchAndBound(const vector<vector<int>> &cost) {
    int n = (int)cost.size();
    TspResult res;
    if (n == 1) { res.bestCost = 0; res.tour = {0, 0}; res.nodesExplored = 1; res.memoryBytes = sizeof(int); return res; }

    //  upper bound: Nearest Neighbour tour 
    TspResult nn = tspNearestNeighbour(cost);
    long long upper = nn.bestCost;
    res.bestCost = upper;
    res.tour = nn.tour;

    TspNode root;
    root.matrix.assign((size_t)n * n, 0);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) root.matrix[i * n + j] = (i == j) ? TSP_INF : cost[i][j];
    root.bound = reduceMatrix(root.matrix, n);
    root.path = {0};
    root.city = 0;

    priority_queue<TspNode, vector<TspNode>, TspNodeCmp> pq;
    pq.push(root);
    res.nodesExplored = 1;
    size_t peakFrontier = 1;

    while (!pq.empty()) {
        peakFrontier = max(peakFrontier, pq.size());
        TspNode cur = pq.top(); pq.pop();
        if (cur.bound >= upper) break;           // best-first: >= upper

        if ((int)cur.path.size() == n) {         // , bound = actual cost
            upper = cur.bound;
            res.bestCost = cur.bound;
            res.tour = cur.path;
            res.tour.push_back(0);
            break;
        }
        int i = cur.city;
        for (int j = 0; j < n; j++) {
            if (cur.matrix[i * n + j] == TSP_INF) continue;       // j visited hai ya edge nahi
            TspNode child;
            child.matrix = cur.matrix;
            child.path = cur.path;
            child.path.push_back(j);
            child.city = j;
            long long edge = cur.matrix[i * n + j];
            for (int k = 0; k < n; k++) { child.matrix[i * n + k] = TSP_INF; child.matrix[k * n + j] = TSP_INF; }
            child.matrix[j * n + 0] = TSP_INF;                    
            child.bound = cur.bound + edge + reduceMatrix(child.matrix, n);
            res.nodesExplored++;
            if (child.bound < upper) pq.push(move(child));        
        }
    }
    long long nodeBytes = (long long)n * n * sizeof(int) + (long long)n * sizeof(int) + sizeof(TspNode);
    res.memoryBytes = (long long)peakFrontier * nodeBytes;
    return res;
}

// ---------------------------------------------------------------- Nearest Neighbour heuristic
inline TspResult tspNearestNeighbour(const vector<vector<int>> &cost) {
    int n = (int)cost.size();
    TspResult res;
    vector<bool> visited(n, false);
    int cur = 0;
    visited[0] = true;
    res.tour.push_back(0);
    long long total = 0;
    for (int step = 1; step < n; step++) {
        int next = -1;
        for (int j = 0; j < n; j++) {
            res.nodesExplored++;
            if (!visited[j] && (next == -1 || cost[cur][j] < cost[cur][next])) next = j;
        }
        visited[next] = true;
        total += cost[cur][next];
        res.tour.push_back(next);
        cur = next;
    }
    total += cost[cur][0];
    res.tour.push_back(0);
    res.bestCost = total;
    res.memoryBytes = (long long)n * (sizeof(int) + 1);
    return res;
}
