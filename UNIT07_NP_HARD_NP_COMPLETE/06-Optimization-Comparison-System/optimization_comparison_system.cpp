/*
 * Program : Optimization Algorithm Comparison System  (Unit 7 Application)
 * Unit    : 7 - NP-Hard / NP-Complete
 *
 * Ek menu-driven system jo 0/1 Knapsack aur TSP ke liye saare algorithms ek saath chalata hai:
 *   Knapsack : Exhaustive | Dynamic Programming | Branch & Bound | Greedy (heuristic)
 *   TSP      : Exhaustive | Held-Karp DP        | Branch & Bound | Nearest Neighbour (heuristic)
 * Har algorithm ke liye table me dikhata hai:
 *   solution value/cost, solution quality (optimal ka %), time, explored nodes, memory, optimal hai ya nahi
 * Scalability benchmark input size badhake dekhta hai ki kaun sa algorithm kahan tak practical hai.
 *
 * Build : g++ -std=c++17 -O2 -I../common optimization_comparison_system.cpp -o optimizer
 */
#include "io.hpp"

// ---- Table ki ek row
struct Row {
    string algorithm;
    long long objective;    // knapsack: value (max), TSP: cost (min)
    double timeUs;
    long long nodes;
    long long memoryBytes;
    bool ran;               // false => skip hua (input bahut bada)
    string note;
};

// Quality % = optimal se kitna paas. Knapsack: value/optimal, TSP: optimal/cost. 100% => optimal
double quality(bool maximize, long long objective, long long optimal) {
    if (optimal <= 0 || objective <= 0) return 100.0;
    return maximize ? 100.0 * objective / optimal : 100.0 * optimal / objective;
}

void printTable(const vector<Row> &rows, bool maximize, long long optimal) {
    cout << "\n" << left << setw(20) << "Algorithm" << setw(11) << (maximize ? "Value" : "Cost") << setw(10) << "Quality"
         << setw(13) << "Time" << setw(14) << "Nodes" << setw(12) << "Memory" << "Optimal?\n" << string(86, '-') << "\n";
    for (const Row &r : rows) {
        cout << setw(20) << r.algorithm;
        if (!r.ran) { cout << r.note << "\n"; continue; }
        double q = quality(maximize, r.objective, optimal);
        char qbuf[16];
        snprintf(qbuf, sizeof(qbuf), "%.1f%%", q);
        cout << setw(11) << r.objective << setw(10) << qbuf << setw(13) << humanMicros(r.timeUs) << setw(14) << r.nodes
             << setw(12) << humanBytes(r.memoryBytes) << (r.objective == optimal ? "YES" : "no") << "\n";
    }
    cout << right;
}

// ---------------------------------------------------------------- Knapsack comparison
vector<Row> compareKnapsack(const vector<Item> &items, int capacity, long long &optimal) {
    int n = (int)items.size();
    vector<Row> rows;
    KnapsackResult r;
    double us;

    if (n <= 24) {
        us = measureMicros([&] { r = knapsackExhaustive(items, capacity); });
        rows.push_back({"Exhaustive", r.bestValue, us, r.nodesExplored, r.memoryBytes, true, ""});
    } else rows.push_back({"Exhaustive", 0, 0, 0, 0, false, "skipped (n > 24, 2^n bahut bada)"});

    if ((long long)(n + 1) * (capacity + 1) <= 120000000LL) {
        us = measureMicros([&] { r = knapsackDP(items, capacity); });
        rows.push_back({"Dynamic Programming", r.bestValue, us, r.nodesExplored, r.memoryBytes, true, ""});
    } else rows.push_back({"Dynamic Programming", 0, 0, 0, 0, false, "skipped (DP table bahut bada)"});

    us = measureMicros([&] { r = knapsackBranchAndBound(items, capacity); });
    if (r.aborted) rows.push_back({"Branch and Bound", 0, 0, 0, 0, false, "aborted (node limit - mushkil input)"});
    else rows.push_back({"Branch and Bound", r.bestValue, us, r.nodesExplored, r.memoryBytes, true, ""});

    us = measureMicros([&] { r = knapsackGreedy(items, capacity); });
    rows.push_back({"Greedy (heuristic)", r.bestValue, us, r.nodesExplored, r.memoryBytes, true, ""});

    optimal = 0;                                  // koi bhi exact method ka best value = optimal
    for (int i = 0; i < 3; i++) if (rows[i].ran) optimal = max(optimal, rows[i].objective);
    return rows;
}

// ---------------------------------------------------------------- TSP comparison
vector<Row> compareTsp(const vector<vector<int>> &cost, long long &optimal) {
    int n = (int)cost.size();
    vector<Row> rows;
    TspResult r;
    double us;

    if (n <= 10) {
        us = measureMicros([&] { r = tspExhaustive(cost); });
        rows.push_back({"Exhaustive", r.bestCost, us, r.nodesExplored, r.memoryBytes, true, ""});
    } else rows.push_back({"Exhaustive", 0, 0, 0, 0, false, "skipped (n > 10, (n-1)! bahut bada)"});

    if (n <= 20) {
        us = measureMicros([&] { r = tspDP(cost); });
        rows.push_back({"Held-Karp DP", r.bestCost, us, r.nodesExplored, r.memoryBytes, true, ""});
    } else rows.push_back({"Held-Karp DP", 0, 0, 0, 0, false, "skipped (n > 20, memory)"});

    if (n <= 16) {
        us = measureMicros([&] { r = tspBranchAndBound(cost); });
        rows.push_back({"Branch and Bound", r.bestCost, us, r.nodesExplored, r.memoryBytes, true, ""});
    } else rows.push_back({"Branch and Bound", 0, 0, 0, 0, false, "skipped (n > 16, frontier memory)"});

    us = measureMicros([&] { r = tspNearestNeighbour(cost); });
    rows.push_back({"Nearest Neighbour", r.bestCost, us, r.nodesExplored, r.memoryBytes, true, ""});

    optimal = LLONG_MAX;
    for (int i = 0; i < 3; i++) if (rows[i].ran) optimal = min(optimal, rows[i].objective);
    if (optimal == LLONG_MAX) optimal = rows[3].objective;     // sirf heuristic chala
    return rows;
}

// ---------------------------------------------------------------- Scalability benchmark
void writeRows(ofstream &out, int n, const vector<Row> &rows, bool maximize, long long optimal) {
    for (const Row &r : rows) {
        if (!r.ran) continue;
        out << n << "," << r.algorithm << "," << r.objective << "," << quality(maximize, r.objective, optimal) << ","
            << r.timeUs << "," << r.nodes << "," << r.memoryBytes << "\n";
    }
}

void runScalability() {
    const string header = "n,algorithm,objective,quality_pct,time_us,nodes,memory_bytes";
    {
        ofstream out = openCsv("results/scalability_knapsack.csv", header);
        cout << "\nKnapsack scalability (random inputs):\n";
        for (int n : {10, 14, 18, 22, 30, 50, 100, 200, 500}) {
            seedRng(777 + n);
            vector<Item> items = randomKnapsackItems(n, 100, 100);
            int capacity = n * 25;
            long long opt = 0;
            vector<Row> rows = compareKnapsack(items, capacity, opt);
            writeRows(out, n, rows, true, opt);
            cout << "  n=" << setw(3) << n << " done (optimal value " << opt << ")\n";
        }
    }
    {
        ofstream out = openCsv("results/scalability_tsp.csv", header);
        cout << "\nTSP scalability (random Euclidean cities):\n";
        for (int n : {5, 7, 9, 10, 12, 14, 16, 18, 20}) {
            seedRng(888 + n);
            vector<vector<int>> cost = randomTspInstance(n);
            long long opt = 0;
            vector<Row> rows = compareTsp(cost, opt);
            writeRows(out, n, rows, false, opt);
            cout << "  n=" << setw(3) << n << " done (optimal cost " << opt << ")\n";
        }
    }
    cout << "\nResults 'results/' me save ho gaye. Graph: python3 ../tools/plot_results.py 06-Optimization-Comparison-System\n";
}

int main() {
    while (true) {
        cout << "\n===== Optimization Algorithm Comparison System =====\n"
                "1. 0/1 Knapsack - manual input\n2. 0/1 Knapsack - random instance\n"
                "3. TSP - manual input\n4. TSP - random instance\n"
                "5. Scalability benchmark (CSV)\n0. Exit\n";
        int choice = readIntInRange("Choice: ", 0, 5);
        if (choice == 0) break;
        if (choice == 5) { runScalability(); continue; }

        long long optimal = 0;
        if (choice <= 2) {
            vector<Item> items;
            int capacity;
            if (choice == 1) readKnapsackInput(items, capacity, 1000);
            else {
                int n = readIntInRange("n (1 se 1000): ", 1, 1000);
                items = randomKnapsackItems(n, 100, 100);
                capacity = n * 25;
            }
            if (items.size() <= 15) printItems(items, capacity);
            vector<Row> rows = compareKnapsack(items, capacity, optimal);   // pehle chalao, phir optimal use karo
            printTable(rows, true, optimal);
        } else {
            vector<vector<int>> cost;
            if (choice == 3) readTspInput(cost, 30);
            else cost = randomTspInstance(readIntInRange("n (2 se 30): ", 2, 30));
            if (cost.size() <= 10) printCostMatrix(cost);
            vector<Row> rows = compareTsp(cost, optimal);
            printTable(rows, false, optimal);
        }
    }
    return 0;
}
