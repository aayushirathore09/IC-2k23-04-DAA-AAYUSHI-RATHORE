/*
 * Program : Merge Sort
 * Unit    : 2 - Divide and Conquer
 *
 * Divide  : array ko mid pe do halves me baanto
 * Conquer : dono halves ko recursively sort karo
 * Combine : do sorted halves ko merge karo
 *
 * Time    : Best/Average/Worst O(n log n)   Recurrence: T(n) = 2T(n/2) + O(n)
 * Space   : O(n) (temporary array)
 * Stable  : Yes
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
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

// COMBINE: a[low..mid] aur a[mid+1..high] (dono sorted) ko merge karo
void merge(vector<int> &a, int low, int mid, int high) {
    vector<int> temp;
    temp.reserve(high - low + 1);
    int i = low, j = mid + 1;

    while (i <= mid && j <= high) {
        if (a[i] <= a[j]) temp.push_back(a[i++]);   // <= se sort stable rehta hai
        else              temp.push_back(a[j++]);
    }
    while (i <= mid)  temp.push_back(a[i++]);       // left half ke bache elements
    while (j <= high) temp.push_back(a[j++]);       // right half ke bache elements

    for (int k = 0; k < (int)temp.size(); k++) a[low + k] = temp[k];
}

void mergeSort(vector<int> &a, int low, int high) {
    if (low >= high) return;                 // base case: 0 ya 1 element
    int mid = low + (high - low) / 2;        // DIVIDE
    mergeSort(a, low, mid);                  // CONQUER left
    mergeSort(a, mid + 1, high);             // CONQUER right
    merge(a, low, mid, high);                // COMBINE
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
    mergeSort(a, 0, n - 1);
    printArray("After : ", a);
    cout << (isSorted(a) ? "Verification: Sorted correctly.\n" : "Verification: FAILED!\n");
    return 0;
}
