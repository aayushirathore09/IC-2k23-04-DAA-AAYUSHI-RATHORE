/*
 * Program : Performance Analysis - Backtracking vs Exhaustive Search (N-Queens)
 * Unit    : 6 - Backtracking
 *
 * For every N the program:
 *   1. counts all solutions with BACKTRACKING (pruned search), counts explored nodes, times it;
 *   2. counts all solutions with EXHAUSTIVE SEARCH (tries every one-queen-per-row
 *      placement, i.e. N^N configurations, and checks each completely), times it
 *      (only for N <= 8, it becomes far too slow after that);
 *   3. verifies that both methods give the same number of solutions;
 *   4. writes everything to backtracking_analysis.csv (used by plot_results.py to draw graphs).
 *
 * Search space sizes: N^N (exhaustive), N! (one queen per row AND column), and the much
 * smaller number of nodes that backtracking actually visits.
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <chrono>
#include <cstdlib>
using namespace std;

const int MIN_N = 4;
const int MAX_N = 12;            // backtracking is run up to this N
const int MAX_EXHAUSTIVE_N = 8;  // exhaustive search is run up to this N only

// ------------------------------ Backtracking ------------------------------
struct BacktrackResult {
    long long solutions = 0;
    long long nodes = 0;
};

void backtrack(int row, int n, vector<bool> &col, vector<bool> &diag1, vector<bool> &diag2,
               BacktrackResult &res) {
    res.nodes++;
    if (row == n) { res.solutions++; return; }
    for (int c = 0; c < n; c++) {
        if (col[c] || diag1[row + c] || diag2[row - c + n - 1]) continue;   // prune
        col[c] = diag1[row + c] = diag2[row - c + n - 1] = true;
        backtrack(row + 1, n, col, diag1, diag2, res);
        col[c] = diag1[row + c] = diag2[row - c + n - 1] = false;           // backtrack
    }
}

BacktrackResult runBacktracking(int n) {
    BacktrackResult res;
    vector<bool> col(n, false), diag1(2 * n - 1, false), diag2(2 * n - 1, false);
    backtrack(0, n, col, diag1, diag2, res);
    return res;
}

// ------------------------------ Exhaustive search ------------------------------
// A complete placement is valid if no two queens share a column or a diagonal.
bool isValidPlacement(const vector<int> &col, int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (col[i] == col[j] || abs(col[i] - col[j]) == j - i) return false;
    return true;
}

// Tries ALL N^N assignments of a column to each row (like an odometer) and checks each one.
long long runExhaustive(int n, long long &configsChecked) {
    vector<int> col(n, 0);
    long long solutions = 0;
    configsChecked = 0;
    while (true) {
        configsChecked++;
        if (isValidPlacement(col, n)) solutions++;

        // move to the next configuration
        int i = n - 1;
        while (i >= 0) {
            col[i]++;
            if (col[i] < n) break;
            col[i] = 0;
            i--;
        }
        if (i < 0) break;      // every configuration has been tried
    }
    return solutions;
}

int main() {
    ofstream csv("backtracking_analysis.csv");
    if (!csv) {
        cerr << "Error: could not create the CSV file.\n";
        return 1;
    }
    csv << "N,solutions,search_space_N_pow_N,backtracking_nodes,backtracking_ms,exhaustive_configs,exhaustive_ms\n";

    cout << left << setw(4) << "N" << setw(11) << "Solutions" << setw(16) << "N^N"
         << setw(14) << "BT nodes" << setw(12) << "BT ms"
         << setw(16) << "Exhaustive cfgs" << setw(14) << "Exhaustive ms" << "Match\n";
    cout << string(95, '-') << "\n";

    for (int n = MIN_N; n <= MAX_N; n++) {
        auto t1 = chrono::high_resolution_clock::now();
        BacktrackResult bt = runBacktracking(n);
        auto t2 = chrono::high_resolution_clock::now();
        double btMs = chrono::duration<double, milli>(t2 - t1).count();

        long long searchSpace = (long long)llround(pow((double)n, (double)n));

        cout << left << setw(4) << n << setw(11) << bt.solutions << setw(16) << searchSpace
             << setw(14) << bt.nodes << setw(12) << fixed << setprecision(4) << btMs;
        csv << n << "," << bt.solutions << "," << searchSpace << "," << bt.nodes << "," << btMs << ",";

        if (n <= MAX_EXHAUSTIVE_N) {
            long long configs = 0;
            auto t3 = chrono::high_resolution_clock::now();
            long long exSolutions = runExhaustive(n, configs);
            auto t4 = chrono::high_resolution_clock::now();
            double exMs = chrono::duration<double, milli>(t4 - t3).count();

            cout << setw(16) << configs << setw(14) << exMs << (exSolutions == bt.solutions ? "OK" : "MISMATCH!") << "\n";
            csv << configs << "," << exMs << "\n";
        } else {
            cout << setw(16) << "-" << setw(14) << "-" << "-\n";
            csv << ",\n";
        }
    }
    csv.close();
    cout << "\nResults saved to backtracking_analysis.csv\n";
    return 0;
}
