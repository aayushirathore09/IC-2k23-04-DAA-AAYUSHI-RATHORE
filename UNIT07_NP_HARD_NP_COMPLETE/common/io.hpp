/*
 * File : io.hpp
 * Unit : 7 - NP-Hard / NP-Complete
 */
#pragma once
#include <iomanip>
#include "knapsack.hpp"
#include "tsp.hpp"
#include "utils.hpp"

// ---------------------------------------------------------------- Knapsack
inline void readKnapsackInput(vector<Item> &items, int &capacity, int maxN) {
    int n = readIntInRange("Items ki sankhya (1 se " + to_string(maxN) + "): ", 1, maxN);
    capacity = readIntInRange("Knapsack capacity W (1 se 100000): ", 1, 100000);
    items.assign(n, Item{});
    cout << "Har item ke liye: weight value  (weight >= 1, value >= 0)\n";
    for (int i = 0; i < n; i++) {
        items[i].weight = readIntInRange("Item " + to_string(i + 1) + " weight: ", 1, 100000);
        items[i].value = readIntInRange("Item " + to_string(i + 1) + " value : ", 0, 1000000);
    }
}

inline void printItems(const vector<Item> &items, int capacity) {
    cout << "\nCapacity W = " << capacity << "\n  #   weight   value\n";
    for (size_t i = 0; i < items.size(); i++)
        cout << setw(3) << i + 1 << setw(8) << items[i].weight << setw(8) << items[i].value << "\n";
}

inline void printKnapsackResult(const string &name, const vector<Item> &items, const KnapsackResult &r) {
    long long w = 0;
    cout << name << ":\n  Chune gaye items (1-based):";
    for (int i : r.chosen) { cout << " " << i + 1; w += items[i].weight; }
    cout << "\n  Total weight = " << w << ", Total value = " << r.bestValue
         << "\n  Nodes/states explored = " << r.nodesExplored << "\n";
}

// ---------------------------------------------------------------- TSP
inline void readTspInput(vector<vector<int>> &cost, int maxN) {
    int n = readIntInRange("Cities ki sankhya (2 se " + to_string(maxN) + "): ", 2, maxN);
    cost.assign(n, vector<int>(n, 0));
    cout << "Cost matrix dalo (" << n << "x" << n << "), diagonal 0 rakho, baaki cost >= 1.\n";
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            if (i == j) { cost[i][j] = 0; continue; }
            cost[i][j] = readIntInRange("cost[" + to_string(i) + "][" + to_string(j) + "]: ", 0, 1000000);
        }
}

inline void printCostMatrix(const vector<vector<int>> &cost) {
    int n = (int)cost.size();
    cout << "\nCost matrix:\n     ";
    for (int j = 0; j < n; j++) cout << setw(5) << j;
    cout << "\n";
    for (int i = 0; i < n; i++) {
        cout << setw(4) << i << " ";
        for (int j = 0; j < n; j++) cout << setw(5) << cost[i][j];
        cout << "\n";
    }
}

inline void printTspResult(const string &name, const TspResult &r) {
    cout << name << ":\n  Tour: ";
    for (size_t i = 0; i < r.tour.size(); i++) cout << r.tour[i] << (i + 1 < r.tour.size() ? " -> " : "");
    cout << "\n  Minimum cost = " << r.bestCost << "\n  Nodes/states explored = " << r.nodesExplored << "\n";
}

inline long long factorialCapped(int n) {          // (n)! ya cap
    long long f = 1;
    for (int i = 2; i <= n; i++) {
        if (f > (long long)4e18 / i) return LLONG_MAX;
        f *= i;
    }
    return f;
}
