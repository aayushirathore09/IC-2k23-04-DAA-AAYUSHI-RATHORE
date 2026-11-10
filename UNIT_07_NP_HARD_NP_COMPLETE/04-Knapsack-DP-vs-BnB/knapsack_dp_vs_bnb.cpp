/*
 * Program : 0/1 Knapsack - Dynamic Programming vs Branch and Bound
 * Unit    : 7 - NP-Hard / NP-Complete
 *
 * DP  : O(n*W) time/space. "Pseudo-polynomial" - W badhne par table bada hota hai.
 * B&B : worst case O(2^n), par achhe bound ki wajah se aksar bahut kam nodes explore karta hai.
 * Dono ka optimal value same aana chahiye (verification har run me hota hai).
 *
 * Benchmark do tarah ke inputs par chalta hai:
 *   random      : weight, value independent  -> B&B ke liye aasan
 *   correlated  : value = weight + 10        -> bound kamzor, B&B ke liye mushkil
 * Metrics   : execution time, explored nodes/cells, memory (estimated), values_match
 * Build     : g++ -std=c++17 -O2 -I../common knapsack_dp_vs_bnb.cpp -o knap_cmp
 */
#include "io.hpp"

void compareOnce(const vector<Item> &items, int capacity) {
    KnapsackResult dp, bb;
    double dpUs = measureMicros([&] { dp = knapsackDP(items, capacity); });
    double bbUs = measureMicros([&] { bb = knapsackBranchAndBound(items, capacity); });
    cout << "\n" << left << setw(18) << "Method" << setw(12) << "Value" << setw(14) << "Time"
         << setw(16) << "Nodes/Cells" << "Memory\n" << string(70, '-') << "\n";
    cout << setw(18) << "Dynamic Prog." << setw(12) << dp.bestValue << setw(14) << humanMicros(dpUs)
         << setw(16) << dp.nodesExplored << humanBytes(dp.memoryBytes) << "\n";
    cout << setw(18) << "Branch & Bound" << setw(12) << bb.bestValue << setw(14) << humanMicros(bbUs)
         << setw(16) << bb.nodesExplored << humanBytes(bb.memoryBytes) << "\n" << right;
    cout << "\nVerification: " << (dp.bestValue == bb.bestValue ? "MATCH - dono ka optimal same hai" : "MISMATCH - bug hai!") << "\n";
}

void runBenchmark() {
    const int instances = 5;                    // har n par 5 random instances ka average
    // random inputs bade n tak chal jate hain; correlated me B&B jaldi phat jata hai, isliye chhote n
    const vector<int> randomSizes = {10, 20, 50, 100, 200, 300, 500};
    const vector<int> correlatedSizes = {10, 20, 30, 40, 50, 60, 80, 100};
    ofstream out = openCsv("results/knapsack_dp_vs_bnb.csv",
        "type,n,capacity,dp_time_us,bnb_time_us,dp_cells,bnb_nodes,dp_memory_bytes,bnb_memory_bytes,values_match,bnb_aborted");
    for (int type = 0; type < 2; type++) {
        string name = type == 0 ? "random" : "correlated";
        cout << "\n[" << name << "]\n   n   capacity      DP time    B&B time     B&B nodes  match\n";
        for (int n : (type == 0 ? randomSizes : correlatedSizes)) {
            seedRng(1000 + n);
            int capacity = n * 25;
            double dpT = 0, bbT = 0, dpC = 0, bbN = 0, dpM = 0, bbM = 0;
            bool allMatch = true, anyAborted = false;
            for (int k = 0; k < instances; k++) {
                vector<Item> items = type == 0 ? randomKnapsackItems(n, 100, 100) : correlatedKnapsackItems(n, 100);
                KnapsackResult dp, bb;
                dpT += measureMicros([&] { dp = knapsackDP(items, capacity); });
                bbT += measureMicros([&] { bb = knapsackBranchAndBound(items, capacity); });
                dpC += dp.nodesExplored; bbN += bb.nodesExplored;
                dpM += dp.memoryBytes;   bbM += bb.memoryBytes;
                anyAborted = anyAborted || bb.aborted;
                allMatch = allMatch && (bb.aborted || dp.bestValue == bb.bestValue);
            }
            out << name << "," << n << "," << capacity << "," << dpT / instances << "," << bbT / instances << ","
                << (long long)(dpC / instances) << "," << (long long)(bbN / instances) << ","
                << (long long)(dpM / instances) << "," << (long long)(bbM / instances) << "," << (allMatch ? 1 : 0) << "," << (anyAborted ? 1 : 0) << "\n";
            cout << setw(4) << n << setw(10) << capacity << setw(13) << humanMicros(dpT / instances)
                 << setw(12) << humanMicros(bbT / instances) << setw(14) << (long long)(bbN / instances)
                 << (anyAborted ? "   ABORTED(node limit)" : (allMatch ? "   yes" : "   NO")) << "\n";
        }
    }
    cout << "\nResults 'results/knapsack_dp_vs_bnb.csv' me save ho gaye. Graph: python3 ../tools/plot_results.py 04-Knapsack-DP-vs-BnB\n";
}

int main() {
    while (true) {
        cout << "\n===== 0/1 Knapsack: DP vs Branch and Bound =====\n1. Manual input compare\n2. Random instance compare\n"
                "3. Benchmark (CSV)\n0. Exit\n";
        int choice = readIntInRange("Choice: ", 0, 3);
        if (choice == 0) break;
        if (choice == 3) { runBenchmark(); continue; }
        vector<Item> items;
        int capacity;
        if (choice == 1) readKnapsackInput(items, capacity, 1000);
        else {
            int n = readIntInRange("n (1 se 1000): ", 1, 1000);
            items = randomKnapsackItems(n, 100, 100);
            capacity = n * 25;
        }
        if (items.size() <= 20) printItems(items, capacity);
        compareOnce(items, capacity);
    }
    return 0;
}
