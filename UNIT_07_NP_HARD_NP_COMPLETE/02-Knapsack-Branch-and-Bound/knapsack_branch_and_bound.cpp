/*
 * Program : 0/1 Knapsack using Branch and Bound (best-first / least-cost search)
 * Unit    : 7 - NP-Hard / NP-Complete
 *
 * Branching : har item ke liye 2 choices -> include / exclude  (state-space tree)
 * Bounding  : bacha hua capacity fractional knapsack se bharke UPPER BOUND nikalo.
 *             Agar bound <= abhi tak ka best answer, to us node ko prune kar do.
 * Search    : priority queue - sabse bade bound wala node pehle expand hota hai.
 * Time      : worst case O(2^n), par pruning ki wajah se practice me bahut kam
 * Space     : O(2^n) worst case (nodes ka pool)
 * Verify    : answer ko Dynamic Programming se match karke check karte hain.
 * Build     : g++ -std=c++17 -O2 -I../common knapsack_branch_and_bound.cpp -o knapsack_bb
 */
#include "io.hpp"

const int MAX_N = 2000;

int main() {
    cout << "===== 0/1 Knapsack - Branch and Bound =====\n1. Manual input\n2. Random instance\n";
    int mode = readIntInRange("Choice: ", 1, 2);
    vector<Item> items;
    int capacity;
    if (mode == 1) {
        readKnapsackInput(items, capacity, MAX_N);
    } else {
        int n = readIntInRange("n (1 se " + to_string(MAX_N) + "): ", 1, MAX_N);
        items = randomKnapsackItems(n, 100, 100);
        capacity = max(1, n * 25);
    }
    if (items.size() <= 30) printItems(items, capacity);
    else cout << "\n" << items.size() << " items, capacity " << capacity << " (itne bade input ki list print nahi ki)\n";

    Timer t;
    KnapsackResult bb = knapsackBranchAndBound(items, capacity);
    double us = t.elapsedMicros();
    printKnapsackResult("\nBranch and Bound", items, bb);

    int n = (int)items.size();
    cout << "  Time = " << humanMicros(us) << "\n";
    if (n < 62) cout << "  Poora state-space tree = 2^(n+1)-1 = " << ((1LL << (n + 1)) - 1) << " nodes\n";
    cout << "  Estimated memory = " << humanBytes(bb.memoryBytes) << "\n";

    // Verification: DP se compare (agar DP table bahut bada na ho)
    if ((long long)(n + 1) * (capacity + 1) <= 100000000LL) {
        KnapsackResult dp = knapsackDP(items, capacity);
        cout << "\nVerification (DP optimum = " << dp.bestValue << "): "
             << (dp.bestValue == bb.bestValue ? "MATCH - answer sahi hai" : "MISMATCH - bug hai!") << "\n";
        return dp.bestValue == bb.bestValue ? 0 : 1;
    }
    cout << "\nDP table bahut bada hai, verification skip kiya.\n";
    return 0;
}
