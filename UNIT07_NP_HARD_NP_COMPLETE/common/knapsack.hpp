/*
 * File : knapsack.hpp
 * Unit : 7 - NP-Hard / NP-Complete
 * Kaam : 0/1 Knapsack ke 4 solvers: Exhaustive, Dynamic Programming,
 *        Branch & Bound (best-first), aur Greedy heuristic (solution quality compare ke liye)
 * Rule : weight >= 1, value >= 0, capacity >= 1 (integers)
 */
#pragma once
#include "utils.hpp"
#include <algorithm>
#include <queue>
#include <vector>
using namespace std;

struct Item {
    int weight;
    int value;
};

// Har solver ka result + statistics
struct KnapsackResult {
    long long bestValue = 0;
    vector<int> chosen;            // original item indices (0-based)
    long long nodesExplored = 0;   // search nodes / DP cells / subsets
    long long memoryBytes = 0;     // data structure ki estimated peak memory
    bool aborted = false;          // B&B: node limit cross hui to true (answer optimal nahi hoga)
};

// ---------------------------------------------------------------- Exhaustive
// Saare 2^n subsets check karo. n <= 30 hona chahiye.
inline KnapsackResult knapsackExhaustive(const vector<Item> &items, int capacity) {
    int n = (int)items.size();
    KnapsackResult res;
    unsigned long long bestMask = 0;
    for (unsigned long long mask = 0; mask < (1ULL << n); mask++) {
        long long w = 0, v = 0;
        for (int i = 0; i < n; i++)
            if (mask & (1ULL << i)) { w += items[i].weight; v += items[i].value; }
        res.nodesExplored++;
        if (w <= capacity && v > res.bestValue) { res.bestValue = v; bestMask = mask; }
    }
    for (int i = 0; i < n; i++)
        if (bestMask & (1ULL << i)) res.chosen.push_back(i);
    res.memoryBytes = (long long)n * sizeof(Item);      // sirf input; extra space O(n)
    return res;
}

// ---------------------------------------------------------------- Dynamic Programming
// dp[i][w] = pehle i items aur capacity w me best value.  Time O(nW), Space O(nW)
inline KnapsackResult knapsackDP(const vector<Item> &items, int capacity) {
    int n = (int)items.size();
    KnapsackResult res;
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = dp[i - 1][w];                                  // item i skip
            if (items[i - 1].weight <= w)                             // item i lo
                dp[i][w] = max(dp[i][w], dp[i - 1][w - items[i - 1].weight] + items[i - 1].value);
            res.nodesExplored++;
        }
    }
    res.bestValue = dp[n][capacity];
    // Traceback: kaun se items liye gaye
    int w = capacity;
    for (int i = n; i >= 1; i--)
        if (dp[i][w] != dp[i - 1][w]) { res.chosen.push_back(i - 1); w -= items[i - 1].weight; }
    reverse(res.chosen.begin(), res.chosen.end());
    res.memoryBytes = (long long)(n + 1) * (capacity + 1) * sizeof(int);
    return res;
}

// ---------------------------------------------------------------- Branch & Bound
// Upper bound: bacha hua capacity fractional knapsack se bharo (relaxation).
// Best-first search: sabse bade bound wala node pehle expand hota hai.
struct KnapNode {
    int level;          // kitne items par decision ho chuka (sorted order me)
    long long profit;
    long long weight;
    double bound;
    int parent;         // node pool me parent index (solution rebuild ke liye)
    bool taken;         // is node pe item liya ya nahi
};

inline double knapsackBound(const vector<Item> &sorted, int capacity, int level, long long profit, long long weight) {
    if (weight > capacity) return 0;
    double bound = (double)profit;
    long long totalWeight = weight;
    int n = (int)sorted.size();
    int i = level;
    while (i < n && totalWeight + sorted[i].weight <= capacity) {
        totalWeight += sorted[i].weight;
        bound += sorted[i].value;
        i++;
    }
    if (i < n) bound += (double)(capacity - totalWeight) * sorted[i].value / sorted[i].weight;
    return bound;
}

// maxNodes: safety limit, taaki mushkil (correlated) input par memory khatam na ho
inline KnapsackResult knapsackBranchAndBound(const vector<Item> &items, int capacity, long long maxNodes = 3000000) {
    int n = (int)items.size();
    KnapsackResult res;

    // value/weight ratio ke hisaab se descending sort (cross-multiplication, floating error nahi)
    vector<int> order(n);
    for (int i = 0; i < n; i++) order[i] = i;
    sort(order.begin(), order.end(), [&](int a, int b) {
        return (long long)items[a].value * items[b].weight > (long long)items[b].value * items[a].weight;
    });
    vector<Item> sorted(n);
    for (int i = 0; i < n; i++) sorted[i] = items[order[i]];

    vector<KnapNode> pool;                       // saare generated nodes
    auto cmp = [&](int a, int b) { return pool[a].bound < pool[b].bound; };
    priority_queue<int, vector<int>, decltype(cmp)> pq(cmp);
    size_t peakFrontier = 0;

    pool.push_back({0, 0, 0, knapsackBound(sorted, capacity, 0, 0, 0), -1, false});
    pq.push(0);
    long long best = 0;
    int bestNode = -1;
    res.nodesExplored = 1;

    while (!pq.empty()) {
        if ((long long)pool.size() > maxNodes) { res.aborted = true; break; }
        peakFrontier = max(peakFrontier, pq.size());
        int curIdx = pq.top(); pq.pop();
        KnapNode cur = pool[curIdx];
        // Bound integer value ke liye floor le sakte hain (profit integer hai)
        if ((long long)(cur.bound + 1e-9) <= best) continue;     // prune
        if (cur.level == n) continue;

        // Child 1: item `level` ko include karo
        KnapNode inc{cur.level + 1, cur.profit + sorted[cur.level].value,
                     cur.weight + sorted[cur.level].weight, 0, curIdx, true};
        res.nodesExplored++;
        if (inc.weight <= capacity) {
            if (inc.profit > best) { best = inc.profit; pool.push_back(inc); bestNode = (int)pool.size() - 1; }
            inc.bound = knapsackBound(sorted, capacity, inc.level, inc.profit, inc.weight);
            if ((long long)(inc.bound + 1e-9) > best) { pool.push_back(inc); pq.push((int)pool.size() - 1); }
        }
        // Child 2: item `level` ko exclude karo
        KnapNode exc{cur.level + 1, cur.profit, cur.weight, 0, curIdx, false};
        res.nodesExplored++;
        exc.bound = knapsackBound(sorted, capacity, exc.level, exc.profit, exc.weight);
        if ((long long)(exc.bound + 1e-9) > best) { pool.push_back(exc); pq.push((int)pool.size() - 1); }
    }

    res.bestValue = best;
    // Best node se root tak chalke chosen items nikalo
    for (int idx = bestNode; idx != -1; idx = pool[idx].parent)
        if (pool[idx].taken) res.chosen.push_back(order[pool[idx].level - 1]);
    sort(res.chosen.begin(), res.chosen.end());
    res.memoryBytes = (long long)pool.size() * sizeof(KnapNode) + (long long)peakFrontier * sizeof(int);
    return res;
}

// ---------------------------------------------------------------- Greedy heuristic
// Ratio ke hisaab se items poore lo ya chhodo. Fast hai, par optimal ki guarantee nahi.
inline KnapsackResult knapsackGreedy(const vector<Item> &items, int capacity) {
    int n = (int)items.size();
    KnapsackResult res;
    vector<int> order(n);
    for (int i = 0; i < n; i++) order[i] = i;
    sort(order.begin(), order.end(), [&](int a, int b) {
        return (long long)items[a].value * items[b].weight > (long long)items[b].value * items[a].weight;
    });
    long long w = 0;
    for (int idx : order) {
        res.nodesExplored++;
        if (w + items[idx].weight <= capacity) {
            w += items[idx].weight;
            res.bestValue += items[idx].value;
            res.chosen.push_back(idx);
        }
    }
    sort(res.chosen.begin(), res.chosen.end());
    res.memoryBytes = (long long)n * sizeof(int);
    return res;
}

// ---------------------------------------------------------------- Random instance
// Strongly-correlated style nahi, simple random: weight 1..maxW, value 1..maxV
inline vector<Item> randomKnapsackItems(int n, int maxWeight, int maxValue) {
    vector<Item> items(n);
    for (auto &it : items) { it.weight = randInt(1, maxWeight); it.value = randInt(1, maxValue); }
    return items;
}

// Strongly correlated instance: value = weight + 10. B&B ke liye mushkil input hota hai
// (bound kamzor ho jata hai), isliye worst-case jaisa behaviour dikhata hai.
inline vector<Item> correlatedKnapsackItems(int n, int maxWeight) {
    vector<Item> items(n);
    for (auto &it : items) { it.weight = randInt(1, maxWeight); it.value = it.weight + 10; }
    return items;
}
