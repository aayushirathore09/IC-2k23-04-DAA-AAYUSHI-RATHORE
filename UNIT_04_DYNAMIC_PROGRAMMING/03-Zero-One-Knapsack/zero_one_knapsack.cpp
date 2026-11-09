/*
 * Program : 0/1 Knapsack using Dynamic Programming (Bottom-Up)
 * Unit    : 4 - Dynamic Programming
 *
 * dp[i][w] = pehle i items use karke, capacity w me milne wali MAX value
 *   Item i na lein : dp[i-1][w]
 *   Item i lein    : dp[i-1][w - weight[i]] + value[i]    (sirf agar weight[i] <= w)
 *   dp[i][w] = max(dono)
 *
 * Time    : O(n * W)
 * Space   : O(n * W)   (selected items nikalne ke liye poori table rakhi hai)
 * Bonus   : Ratio-based Greedy se compare karta hai (0/1 me greedy galat ho sakta hai)
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>
using namespace std;

int readInt(const string &prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Integer dalo: ";
    }
    return value;
}

int readIntInRange(const string &prompt, int minVal, int maxVal) {
    int v = readInt(prompt);
    while (v < minVal || v > maxVal)
        v = readInt("Invalid! " + to_string(minVal) + " se " + to_string(maxVal) + " ke beech dalo: ");
    return v;
}

// DP table banao aur return karo. Items 1-indexed (value[0], weight[0] unused).
vector<vector<long long>> buildKnapsackTable(const vector<int> &value, const vector<int> &weight,
                                             int n, int W) {
    vector<vector<long long>> dp(n + 1, vector<long long>(W + 1, 0));
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= W; w++) {
            dp[i][w] = dp[i - 1][w];                                   // item i nahi liya
            if (weight[i] <= w)                                        // item i le sakte hain
                dp[i][w] = max(dp[i][w], dp[i - 1][w - weight[i]] + value[i]);
        }
    }
    return dp;
}

// Table me peeche chalke selected items nikalo
vector<int> findSelectedItems(const vector<vector<long long>> &dp, const vector<int> &weight,
                              int n, int W) {
    vector<int> selected;
    int w = W;
    for (int i = n; i >= 1; i--) {
        if (dp[i][w] != dp[i - 1][w]) {       // value badli => item i liya gaya tha
            selected.push_back(i);
            w -= weight[i];
        }
    }
    reverse(selected.begin(), selected.end());
    return selected;
}

// Comparison ke liye: value/weight ratio wala greedy (0/1 me optimal GUARANTEED nahi)
long long greedyKnapsack01(const vector<int> &value, const vector<int> &weight, int n, int W) {
    vector<int> order;
    for (int i = 1; i <= n; i++) order.push_back(i);
    sort(order.begin(), order.end(), [&](int a, int b) {
        return (double)value[a] / weight[a] > (double)value[b] / weight[b];
    });
    long long total = 0;
    int remaining = W;
    for (int i : order) {
        if (weight[i] <= remaining) {          // poora item aaye to hi lo (fraction allowed nahi)
            total += value[i];
            remaining -= weight[i];
        }
    }
    return total;
}

int main() {
    int n = readIntInRange("Items ki sankhya (1 se 100): ", 1, 100);
    int W = readIntInRange("Knapsack capacity (1 se 10000): ", 1, 10000);

    vector<int> value(n + 1), weight(n + 1);
    for (int i = 1; i <= n; i++) {
        value[i]  = readIntInRange("Item " + to_string(i) + " value  (>0): ", 1, 1000000);
        weight[i] = readIntInRange("Item " + to_string(i) + " weight (>0): ", 1, 1000000);
    }

    vector<vector<long long>> dp = buildKnapsackTable(value, weight, n, W);

    // Chhoti table ho to poori print karo (lab report ke liye useful)
    if (W <= 20 && n <= 10) {
        cout << "\nDP Table (rows = items, columns = capacity 0.." << W << "):\n";
        for (int i = 0; i <= n; i++) {
            for (int w = 0; w <= W; w++) cout << setw(5) << dp[i][w];
            cout << "\n";
        }
    }

    vector<int> selected = findSelectedItems(dp, weight, n, W);
    int totalWeight = 0;
    cout << "\nMaximum value (DP) = " << dp[n][W] << "\n";
    cout << "Selected items:\n";
    for (int i : selected) {
        cout << "  Item " << i << " (value " << value[i] << ", weight " << weight[i] << ")\n";
        totalWeight += weight[i];
    }
    if (selected.empty()) cout << "  (koi item nahi aaya)\n";
    cout << "Total weight = " << totalWeight << " / " << W << "\n";

    long long greedy = greedyKnapsack01(value, weight, n, W);
    cout << "\nGreedy (ratio) value = " << greedy << "\n";
    if (greedy < dp[n][W])
        cout << "Greedy yaha optimal nahi tha, DP ne " << dp[n][W] - greedy << " zyada value di.\n";
    else
        cout << "Is input pe greedy bhi optimal nikla.\n";
    return 0;
}
