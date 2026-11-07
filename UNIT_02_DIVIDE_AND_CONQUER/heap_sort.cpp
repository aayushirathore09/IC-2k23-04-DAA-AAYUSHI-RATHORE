/*
 * Program : Heap Sort
 * Unit    : 2 - Divide and Conquer
 *
 * Approach: 1) convert array into max-heap  2) put root in the last, 
 *           reduce heap soze by 1, heapify . Repeat.
 *
 * Time    : Best/Average/Worst O(n log n)   
 * Space   : O(1) (in-place)
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

// Node 'i' ko neeche push karo jab tak max-heap property sahi na ho jaye
// heapSize = heap me abhi kitne elements hain
void heapify(vector<int> &a, int heapSize, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < heapSize && a[left] > a[largest])   largest = left;
    if (right < heapSize && a[right] > a[largest]) largest = right;

    if (largest != i) {
        swap(a[i], a[largest]);
        heapify(a, heapSize, largest);
    }
}

void buildMaxHeap(vector<int> &a) {
    int n = a.size();
    for (int i = n / 2 - 1; i >= 0; i--)     // last non-leaf se root tak
        heapify(a, n, i);
}

void heapSort(vector<int> &a) {
    int n = a.size();
    buildMaxHeap(a);
    for (int end = n - 1; end > 0; end--) {
        swap(a[0], a[end]);                  // max ko end me bhejo
        heapify(a, end, 0);                  // chhote heap ko theek karo
    }
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
    heapSort(a);
    printArray("After : ", a);
    cout << (isSorted(a) ? "Verification: Sorted correctly.\n" : "Verification: FAILED!\n");
    return 0;
}
