/*
 * Program : Traveling Salesperson Problem using Branch and Bound (reduced cost matrix)
 * Unit    : 7 - NP-Hard / NP-Complete
 *
 * Bounding : har node par cost matrix ke har row aur column me se minimum ghatao
 *            (matrix "reduce" karo). Total ghataya hua cost = LOWER BOUND.
 * Branching: current city se agle unvisited city j tak ka edge choose karo; row i, column j
 *            ko INF karo, aur j -> 0 ko INF karo (tour ko beech me band hone se roko).
 * Search   : min-heap (sabse chhota lower bound pehle). Bound >= best known tour ho to prune.
 * Upper bd : Nearest Neighbour tour se shuruaat (pruning jaldi shuru ho jati hai).
 * Time     : worst case O(n! * n^2), practice me bahut kam
 * Space    : har node me n x n matrix -> O(n^2) per node
 * Verify   : n <= 15 par Held-Karp DP se answer match karte hain.
 * Build    : g++ -std=c++17 -O2 -I../common tsp_branch_and_bound.cpp -o tsp_bb
 */
#include "io.hpp"

const int MAX_N = 20;

int main() {
    cout << "===== TSP - Branch and Bound =====\n1. Manual input\n2. Random instance\n";
    int mode = readIntInRange("Choice: ", 1, 2);
    vector<vector<int>> cost;
    if (mode == 1) readTspInput(cost, MAX_N);
    else cost = randomTspInstance(readIntInRange("n (2 se " + to_string(MAX_N) + "): ", 2, MAX_N));
    printCostMatrix(cost);

    Timer t;
    TspResult bb = tspBranchAndBound(cost);
    double us = t.elapsedMicros();
    printTspResult("\nBranch and Bound", bb);
    cout << "  Time = " << humanMicros(us) << "\n  Estimated memory = " << humanBytes(bb.memoryBytes) << "\n";
    int n = (int)cost.size();
    cout << "  Total possible tours (n-1)! = " << factorialCapped(n - 1) << "\n";

    if (tourCost(cost, bb.tour) != bb.bestCost) { cout << "\nINTERNAL ERROR: tour ka cost mismatch!\n"; return 1; }
    if (n <= 15) {
        TspResult dp = tspDP(cost);
        cout << "\nVerification (Held-Karp optimum = " << dp.bestCost << "): "
             << (dp.bestCost == bb.bestCost ? "MATCH - answer sahi hai" : "MISMATCH - bug hai!") << "\n";
        return dp.bestCost == bb.bestCost ? 0 : 1;
    }
    cout << "\nn > 15, verification skip kiya.\n";
    return 0;
}
