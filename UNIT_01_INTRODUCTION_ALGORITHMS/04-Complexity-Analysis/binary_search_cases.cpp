/*
 * Program : Best / Average / Worst case analysis of Binary Search
 * Unit    : 1 - Introduction to Algorithms
 *
 * Method  : Array arr[i] = 2*i (sirf even numbers), isliye odd numbers
 *           (aur -1) array me NAHI hote -> unsuccessful search test ho sakta hai.
 *           1 comparison = ek loop iteration (ek mid element check).
 *
 *   Best    : key = middle element          -> 1 comparison
 *   Average : saare present keys ka average -> ~ log2(n) - 1
 *   Worst   : saare absent keys me maximum  -> floor(log2 n) + 1
 *
 * Output  : console table + case_analysis.csv (graph banane ke liye)
 */
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

// Iterative Binary Search jo comparisons count return karta hai
int binarySearchCount(const vector<int> &arr, int key) {
    int low = 0, high = (int)arr.size() - 1, comparisons = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;
        comparisons++;
        if (arr[mid] == key) break;
        else if (arr[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return comparisons;
}

int main() {
    const int sizes[] = {10, 100, 1000, 10000, 100000};

    ofstream csv("case_analysis.csv");
    if (!csv) {
        cerr << "Error: CSV file nahi ban saki.\n";
        return 1;
    }
    csv << "n,best,average,worst,theoretical_worst\n";

    cout << left << setw(10) << "n" << setw(8) << "Best" << setw(12) << "Average"
         << setw(8) << "Worst" << setw(18) << "floor(log2 n)+1" << "\n";
    cout << string(56, '-') << "\n";

    for (int n : sizes) {
        vector<int> arr(n);
        for (int i = 0; i < n; i++) arr[i] = 2 * i;

        // BEST CASE: middle element search karo
        int best = binarySearchCount(arr, arr[(n - 1) / 2]);

        // AVERAGE CASE: har present key ke liye comparisons ka average
        long long total = 0;
        for (int i = 0; i < n; i++) total += binarySearchCount(arr, arr[i]);
        double average = (double)total / n;

        // WORST CASE: har absent key (-1 aur saare odd numbers) me se maximum
        int worst = binarySearchCount(arr, -1);
        for (int i = 0; i < n; i++)
            worst = max(worst, binarySearchCount(arr, 2 * i + 1));

        int theoretical = (int)floor(log2((double)n)) + 1;

        cout << left << setw(10) << n << setw(8) << best << setw(12) << fixed << setprecision(2)
             << average << setw(8) << worst << setw(18) << theoretical << "\n";
        csv << n << "," << best << "," << average << "," << worst << "," << theoretical << "\n";
    }

    csv.close();
    cout << "\nResults 'case_analysis.csv' me save ho gaye.\n";
    return 0;
}
