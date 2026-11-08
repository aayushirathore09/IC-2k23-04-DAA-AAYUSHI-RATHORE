/*
 * Program : Fractional Knapsack (Greedy)
 * Unit    : 3 - Greedy Method
 *
 * Greedy choice : value/weight ratio MAX.
 *                 
 * Time          : O(n log n)  (sorting)
 * Space         : O(n)
 
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Item {
    int id;
    int value;
    int weight;
    double ratio;      // value / weight
};

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

double fractionalKnapsack(vector<Item> items, int capacity, vector<double> &taken) {
    // Acc to Ratio, descending sort
    sort(items.begin(), items.end(),
         [](const Item &a, const Item &b) { return a.ratio > b.ratio; });

    double totalValue = 0.0;
    int remaining = capacity;
    taken.assign(items.size() + 1, 0.0);      // index = item id

    for (const Item &it : items) {
        if (remaining == 0) break;
        if (it.weight <= remaining) {          // take whole item
            taken[it.id] = 1.0;
            totalValue += it.value;
            remaining -= it.weight;
        } else {                               // can take only fraction
            double fraction = (double)remaining / it.weight;
            taken[it.id] = fraction;
            totalValue += it.value * fraction;
            remaining = 0;
        }
    }
    return totalValue;
}

int main() {
    int n = readIntInRange("Items ki sankhya (1 se 1000): ", 1, 1000);
    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        items[i].id = i + 1;
        items[i].value  = readIntInRange("Item " + to_string(i + 1) + " value  (>0): ", 1, 1000000000);
        items[i].weight = readIntInRange("Item " + to_string(i + 1) + " weight (>0): ", 1, 1000000000);
        items[i].ratio  = (double)items[i].value / items[i].weight;
    }
    int capacity = readIntInRange("Knapsack capacity (>0): ", 1, 1000000000);

    vector<double> taken;
    double best = fractionalKnapsack(items, capacity, taken);

    cout << fixed << setprecision(2);
    cout << "\nItem  Value  Weight  Ratio   Liya (fraction)\n";
    for (const Item &it : items)
        cout << setw(4) << it.id << setw(7) << it.value << setw(8) << it.weight
             << setw(8) << it.ratio << setw(10) << taken[it.id] << "\n";
    cout << "\nMaximum total value = " << best << "\n";
    return 0;
}
