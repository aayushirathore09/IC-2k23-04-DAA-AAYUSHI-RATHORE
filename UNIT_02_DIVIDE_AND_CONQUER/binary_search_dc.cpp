/*
 * Program : Binary Search using Divide and Conquer
 * Unit    : 2 - Divide and Conquer
 *
 * Divide  : divide array into 2 halves from thw mid .
 * Conquer : search in only one half where the key can exist.
 *
 * Time    : Best O(1), Average/Worst O(log n)   Recurrence: T(n) = T(n/2) + O(1)
 * Space   : O(log n) (recursion stack)
 */

#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

const int MAX_SIZE = 100000;

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

bool isSorted(const vector<int> &a) {
    for (size_t i = 1; i < a.size(); i++)
        if (a[i - 1] > a[i]) return false;
    return true;
}

void printArray(const string &label, const vector<int> &a) {
    cout << label;
    if (a.size() > 50) { cout << "(" << a.size() << " elements, print skip)\n"; return; }
    for (int x : a) cout << x << " ";
    cout << "\n";
}

// Divide & Conquer Binary Search on a[low..high]
int binarySearchDC(const vector<int> &a, int low, int high, int key, int &comparisons) {
    if (low > high) return -1;                       // base case: not found
    int mid = low + (high - low) / 2;                // DIVIDE
    comparisons++;
    if (a[mid] == key) return mid;                   // base case: found
    if (a[mid] < key)                                // CONQUER: right half
        return binarySearchDC(a, mid + 1, high, key, comparisons);
    return binarySearchDC(a, low, mid - 1, key, comparisons);  // CONQUER: left half
}

int main() {
    srand((unsigned)time(nullptr));
    vector<int> a;

    cout << "1. Manual input (sorted)\n2. Random sorted array\n";
    int mode = readInt("Choice: ");
    int n = readInt("Elements ki sankhya (1 se " + to_string(MAX_SIZE) + "): ");
    while (n < 1 || n > MAX_SIZE) n = readInt("Invalid size! Dobara dalo: ");

    if (mode == 2) {
        for (int i = 0; i < n; i++) a.push_back(rand() % 1000);
        sort(a.begin(), a.end());       // binary search ke liye sorted zaroori
    } else {
        cout << "Ascending sorted elements dalo:\n";
        for (int i = 0; i < n; i++) a.push_back(readInt("a[" + to_string(i) + "]: "));
        if (!isSorted(a)) {
            cout << "Error: Array sorted nahi hai! Binary Search nahi chal sakta.\n";
            return 1;
        }
    }
    printArray("Array: ", a);

    char again;
    do {
        int key = readInt("Search karne ki value: ");
        int comparisons = 0;
        int idx = binarySearchDC(a, 0, n - 1, key, comparisons);
        if (idx == -1) cout << key << " nahi mila. ";
        else cout << key << " index " << idx << " pe mila. ";
        cout << "(Comparisons: " << comparisons << ")\n";
        cout << "Dobara search? (y/n): ";
        cin >> again;
    } while (again == 'y' || again == 'Y');
    return 0;
}
