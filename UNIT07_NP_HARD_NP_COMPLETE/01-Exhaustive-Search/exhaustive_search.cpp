/*
 * Program : Exhaustive Search (Brute Force) - chhote NP-Hard problems par demo
 * Unit    : 7 - NP-Hard / NP-Complete
 *
 * Problems : (1) 0/1 Knapsack  -> saare 2^n subsets check
 *            (2) TSP           -> saari (n-1)! permutations check
 * Idea     : Exhaustive search hamesha sahi (optimal) answer deta hai, par search space
 *            exponential / factorial hai, isliye sirf chhote n ke liye practical hai.
 * Time     : Knapsack O(n * 2^n),  TSP O(n!)
 * Space    : O(n)
 * Build    : g++ -std=c++17 -O2 -I../common exhaustive_search.cpp -o exhaustive
 */
#include "io.hpp"

const int MAX_KNAPSACK_N = 25;
const int MAX_TSP_N = 11;

void demoKnapsack() {
    cout << "\n1) Manual input  2) Random instance\n";
    int mode = readIntInRange("Choice: ", 1, 2);
    vector<Item> items;
    int capacity;
    if (mode == 1) {
        readKnapsackInput(items, capacity, MAX_KNAPSACK_N);
    } else {
        int n = readIntInRange("n (1 se " + to_string(MAX_KNAPSACK_N) + "): ", 1, MAX_KNAPSACK_N);
        items = randomKnapsackItems(n, 30, 60);
        capacity = max(1, n * 8);
    }
    printItems(items, capacity);
    Timer t;
    KnapsackResult r = knapsackExhaustive(items, capacity);
    double us = t.elapsedMicros();
    printKnapsackResult("Exhaustive Search", items, r);
    cout << "  Time = " << humanMicros(us) << "  (total subsets 2^" << items.size() << " = "
         << (1LL << items.size()) << ")\n";
}

void demoTsp() {
    cout << "\n1) Manual input  2) Random instance\n";
    int mode = readIntInRange("Choice: ", 1, 2);
    vector<vector<int>> cost;
    if (mode == 1) readTspInput(cost, MAX_TSP_N);
    else cost = randomTspInstance(readIntInRange("n (2 se " + to_string(MAX_TSP_N) + "): ", 2, MAX_TSP_N));
    printCostMatrix(cost);
    Timer t;
    TspResult r = tspExhaustive(cost);
    printTspResult("Exhaustive Search", r);
    cout << "  Time = " << humanMicros(t.elapsedMicros()) << "  (total tours (n-1)! = " << factorialCapped((int)cost.size() - 1) << ")\n";
}

// Performance analysis: n badhao, time record karo -> results/*.csv
void runBenchmark() {
    seedRng(12345);
    {
        ofstream out = openCsv("results/exhaustive_knapsack.csv", "n,subsets,time_us");
        cout << "\nKnapsack (n vs time):\n   n     subsets(2^n)        time\n";
        for (int n = 10; n <= 24; n += 2) {
            vector<Item> items = randomKnapsackItems(n, 30, 60);
            int cap = n * 8;
            double us = measureMicros([&] { knapsackExhaustive(items, cap); });
            out << n << "," << (1LL << n) << "," << us << "\n";
            cout << setw(4) << n << setw(16) << (1LL << n) << setw(14) << humanMicros(us) << "\n";
        }
    }
    {
        ofstream out = openCsv("results/exhaustive_tsp.csv", "n,tours,time_us");
        cout << "\nTSP (n vs time):\n   n     tours((n-1)!)        time\n";
        for (int n = 4; n <= 11; n++) {
            vector<vector<int>> cost = randomTspInstance(n);
            double us = measureMicros([&] { tspExhaustive(cost); });
            out << n << "," << factorialCapped(n - 1) << "," << us << "\n";
            cout << setw(4) << n << setw(18) << factorialCapped(n - 1) << setw(14) << humanMicros(us) << "\n";
        }
    }
    cout << "\nResults 'results/' folder me save ho gaye. Graph ke liye: python3 ../tools/plot_results.py 01-Exhaustive-Search\n";
}

int main() {
    while (true) {
        cout << "\n===== Exhaustive Search =====\n1. 0/1 Knapsack (brute force)\n2. TSP (brute force)\n"
                "3. Benchmark (n vs time, CSV)\n0. Exit\n";
        int choice = readIntInRange("Choice: ", 0, 3);
        if (choice == 0) break;
        if (choice == 1) demoKnapsack();
        else if (choice == 2) demoTsp();
        else runBenchmark();
    }
    return 0;
}
