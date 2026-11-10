/*
 * Program : TSP - Dynamic Programming (Held-Karp) vs Branch and Bound
 * Unit    : 7 - NP-Hard / NP-Complete
 *
 * DP (Held-Karp) : O(n^2 * 2^n) time, O(n * 2^n) space - input ki shape par depend nahi karta.
 * B&B            : worst case factorial, par pruning se aksar bahut kam nodes; lekin har node me
 *                  n x n matrix hota hai, isliye memory tezi se badh sakti hai.
 * Dono ka optimal cost same aana chahiye (verification har run me hota hai).
 * Benchmark : Euclidean random cities, har n par 5 instances ka average.
 * Build     : g++ -std=c++17 -O2 -I../common tsp_dp_vs_bnb.cpp -o tsp_cmp
 */
#include "io.hpp"

const int MAX_DP_N = 20;       // Held-Karp memory: n=20 par ~ 50 MB

void compareOnce(const vector<vector<int>> &cost) {
    int n = (int)cost.size();
    TspResult dp, bb;
    double dpUs = measureMicros([&] { dp = tspDP(cost); });
    double bbUs = measureMicros([&] { bb = tspBranchAndBound(cost); });
    cout << "\n" << left << setw(18) << "Method" << setw(12) << "Cost" << setw(14) << "Time"
         << setw(20) << "Nodes/Transitions" << "Memory\n" << string(74, '-') << "\n";
    cout << setw(18) << "Held-Karp DP" << setw(12) << dp.bestCost << setw(14) << humanMicros(dpUs)
         << setw(20) << dp.nodesExplored << humanBytes(dp.memoryBytes) << "\n";
    cout << setw(18) << "Branch & Bound" << setw(12) << bb.bestCost << setw(14) << humanMicros(bbUs)
         << setw(20) << bb.nodesExplored << humanBytes(bb.memoryBytes) << "\n" << right;
    cout << "(Total tours (n-1)! = " << factorialCapped(n - 1) << ")\n";
    cout << "\nVerification: " << (dp.bestCost == bb.bestCost ? "MATCH - dono ka optimal same hai" : "MISMATCH - bug hai!") << "\n";
}

// n=5..16: DP aur B&B dono; n=17..19: sirf DP (B&B ka frontier bahut bada ho jata hai) -> bnb columns = -1
void runBenchmark() {
    const int instances = 5;
    ofstream out = openCsv("results/tsp_dp_vs_bnb.csv",
        "n,dp_time_us,bnb_time_us,dp_transitions,bnb_nodes,dp_memory_bytes,bnb_memory_bytes,costs_match");
    cout << "\n   n      DP time    B&B time     DP transitions   B&B nodes  match\n";
    for (int n = 5; n <= 19; n++) {
        seedRng(5000 + n);
        bool runBnb = n <= 16;
        double dpT = 0, bbT = 0, dpS = 0, bbN = 0, dpM = 0, bbM = 0;
        bool allMatch = true;
        for (int k = 0; k < instances; k++) {
            if (!runBnb && k >= 2) break;                          // bade n par kam instances
            vector<vector<int>> cost = randomTspInstance(n);
            TspResult dp, bb;
            dpT += measureMicros([&] { dp = tspDP(cost); });
            dpS += dp.nodesExplored; dpM += dp.memoryBytes;
            if (runBnb) {
                bbT += measureMicros([&] { bb = tspBranchAndBound(cost); });
                bbN += bb.nodesExplored; bbM += bb.memoryBytes;
                allMatch = allMatch && (dp.bestCost == bb.bestCost);
            }
        }
        int used = runBnb ? instances : 2;
        out << n << "," << dpT / used << "," << (runBnb ? bbT / used : -1) << "," << (long long)(dpS / used) << ","
            << (runBnb ? (long long)(bbN / used) : -1) << "," << (long long)(dpM / used) << ","
            << (runBnb ? (long long)(bbM / used) : -1) << "," << (allMatch ? 1 : 0) << "\n";
        cout << setw(4) << n << setw(13) << humanMicros(dpT / used)
             << setw(12) << (runBnb ? humanMicros(bbT / used) : string("-"))
             << setw(14) << (long long)(dpS / used) << setw(12) << (runBnb ? to_string((long long)(bbN / used)) : string("-"))
             << (allMatch ? "   yes" : "   NO") << "\n";
    }
    cout << "\nResults 'results/tsp_dp_vs_bnb.csv' me save ho gaye. Graph: python3 ../tools/plot_results.py 05-TSP-DP-vs-BnB\n";
}

int main() {
    while (true) {
        cout << "\n===== TSP: Held-Karp DP vs Branch and Bound =====\n1. Manual input compare\n2. Random instance compare\n"
                "3. Benchmark (CSV)\n0. Exit\n";
        int choice = readIntInRange("Choice: ", 0, 3);
        if (choice == 0) break;
        if (choice == 3) { runBenchmark(); continue; }
        vector<vector<int>> cost;
        if (choice == 1) readTspInput(cost, MAX_DP_N);
        else cost = randomTspInstance(readIntInRange("n (2 se 16): ", 2, 16));
        if (cost.size() <= 12) printCostMatrix(cost);
        compareOnce(cost);
    }
    return 0;
}
