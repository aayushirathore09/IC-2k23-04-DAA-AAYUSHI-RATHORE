/*
 * Program : Sum of Subsets using Backtracking
 * Unit    : 6 - Backtracking
 *
 * Problem : Given n positive integers and a target sum M, find ALL subsets whose
 *           elements add up exactly to M.
 *
 * State-space tree: at each item we branch on "include it" / "exclude it" (2^n leaves).
 * Pruning rules (weights are sorted in ascending order):
 *   1. sum + (sum of all remaining items) < M  -> M can no longer be reached, stop.
 *   2. sum + current item > M                   -> this and every later item is too big, stop.
 *
 * Time    : O(2^n) worst case, usually far fewer nodes because of pruning.
 * Space   : O(n) recursion depth.
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

int readInt(const string &prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter an integer: ";
    }
    return value;
}

int readIntInRange(const string &prompt, int minVal, int maxVal) {
    int v = readInt(prompt);
    while (v < minVal || v > maxVal)
        v = readInt("Invalid! Enter a value between " + to_string(minVal) + " and " + to_string(maxVal) + ": ");
    return v;
}

// idx       : index of the item being decided
// sum       : sum of the items chosen so far
// remaining : sum of the items w[idx..n-1] that are still undecided
void sumOfSubsets(const vector<int> &w, int idx, long long sum, long long remaining, long long target,
                  vector<int> &chosen, long long &nodes, vector<vector<int>> &results) {
    nodes++;

    if (sum == target) {                       // a valid subset has been found
        results.push_back(chosen);
        return;                                // weights are positive, adding more only overshoots
    }
    if (idx == (int)w.size()) return;          // no items left
    if (sum + remaining < target) return;      // pruning rule 1
    if (sum + w[idx] > target) return;         // pruning rule 2

    // Branch 1: include w[idx]
    chosen.push_back(w[idx]);
    sumOfSubsets(w, idx + 1, sum + w[idx], remaining - w[idx], target, chosen, nodes, results);
    chosen.pop_back();                         // backtrack

    // Branch 2: exclude w[idx]
    sumOfSubsets(w, idx + 1, sum, remaining - w[idx], target, chosen, nodes, results);
}

int main() {
    int n = readIntInRange("Enter the number of items (1 to 30): ", 1, 30);
    vector<int> w(n);
    for (int i = 0; i < n; i++)
        w[i] = readIntInRange("Item " + to_string(i + 1) + " (positive integer): ", 1, 1000000);
    int target = readIntInRange("Enter the target sum M (positive): ", 1, 1000000000);

    sort(w.begin(), w.end());                  // needed for pruning rule 2

    long long total = 0;
    for (int x : w) total += x;

    vector<int> chosen;
    vector<vector<int>> results;
    long long nodes = 0;
    sumOfSubsets(w, 0, 0, total, target, chosen, nodes, results);

    cout << "\nSorted items: ";
    for (int x : w) cout << x << " ";
    cout << "\nTarget sum M = " << target << "\n";

    if (results.empty()) {
        cout << "No subset adds up to " << target << ".\n";
    } else {
        for (size_t i = 0; i < results.size(); i++) {
            cout << "Solution " << i + 1 << ": { ";
            for (int x : results[i]) cout << x << " ";
            cout << "}\n";
        }
    }
    cout << "Number of solutions = " << results.size() << "\n";
    cout << "Nodes explored      = " << nodes << " (full tree without pruning: " << (1LL << (n + 1)) - 1 << ")\n";
    return 0;
}
