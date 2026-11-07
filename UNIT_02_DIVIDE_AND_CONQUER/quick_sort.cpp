/*
 * Program : Quick Sort
 * Unit    : 2 - Divide and Conquer
 *
 * Divide  : pivot chuno, partition karo (chhote left me, bade right me)
 * Conquer : left aur right partitions ko recursively sort karo
 * Combine : kuch nahi (partition ke baad array in-place sorted hota hai)
 *
 * Time    : Best/Average O(n log n), Worst O(n^2)  (already sorted array + last pivot)
 * Space   : O(log n) average recursion stack
 * Stable  : No
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <utility>
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

// Lomuto partition: last element pivot. Pivot ki final position return karta hai.
int partition(vector<int> &a, int low, int high) {
    int pivot = a[high];
    int i = low - 1;                         // i = chhote elements ki boundary
    for (int j = low; j < high; j++) {
        if (a[j] <= pivot) {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[high]);                 // pivot ko sahi jagah rakho
    return i + 1;
}

void quickSort(vector<int> &a, int low, int high) {
    if (low >= high) return;                 // base case
    int p = partition(a, low, high);         // DIVIDE
    quickSort(a, low, p - 1);                // CONQUER left
    quickSort(a, p + 1, high);               // CONQUER right
}

int main() {
    srand((unsigned)time(nullptr));
    vector<int> a;

    cout << "1. Manual input\n2. Random array\n";
    int mode = readInt("Choice: ");
    int n = readInt("Elements ki sankhya (1 se " + to_string(MAX_SIZE) + "): ");
    while (n < 1 || n > MAX_SIZE) n = readInt("Invalid size! Dobara dalo: ");

    if (mode == 2) {
        for (int i = 0; i < n; i++) a.push_back(rand() % 1000);
    } else {
        for (int i = 0; i < n; i++) a.push_back(readInt("a[" + to_string(i) + "]: "));
    }

    printArray("Before: ", a);
    quickSort(a, 0, n - 1);
    printArray("After : ", a);
    cout << (isSorted(a) ? "Verification: Sorted correctly.\n" : "Verification: FAILED!\n");
    return 0;
}
