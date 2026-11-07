/*
 * Program : Recursive Binary Search
 * Unit    : 1 - Introduction to Algorithms
 * Time    : Best O(1), Average/Worst O(log n)
 * Space   : O(log n)  (recursion call stack)
 */
#include <iostream>
#include <string>
#include <limits>
using namespace std;

const int MAX_SIZE = 1000;

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

bool isSorted(const int arr[], int n) {
    for (int i = 1; i < n; i++)
        if (arr[i - 1] > arr[i]) return false;
    return true;
}

// Recursive Binary Search on arr[low..high]
// Base case: low > high -> element nahi mila
int binarySearchRecursive(const int arr[], int low, int high, int key, int &comparisons) {
    if (low > high) return -1;              // base case: not found

    int mid = low + (high - low) / 2;
    comparisons++;

    if (arr[mid] == key) return mid;        // base case: found
    if (arr[mid] < key)
        return binarySearchRecursive(arr, mid + 1, high, key, comparisons);  // right half
    return binarySearchRecursive(arr, low, mid - 1, key, comparisons);       // left half
}

int main() {
    int arr[MAX_SIZE];

    int n = readInt("Elements ki sankhya (1 se " + to_string(MAX_SIZE) + "): ");
    while (n < 1 || n > MAX_SIZE) n = readInt("Invalid size! Dobara dalo: ");

    cout << "Sorted (ascending) elements dalo:\n";
    for (int i = 0; i < n; i++) arr[i] = readInt("arr[" + to_string(i) + "]: ");

    if (!isSorted(arr, n)) {
        cout << "Error: Array sorted nahi hai! Binary Search nahi chal sakta.\n";
        return 1;
    }

    char again;
    do {
        int key = readInt("Search karne ki value: ");
        int comparisons = 0;
        int idx = binarySearchRecursive(arr, 0, n - 1, key, comparisons);

        if (idx == -1) cout << key << " nahi mila. ";
        else cout << key << " index " << idx << " pe mila. ";
        cout << "(Comparisons: " << comparisons << ")\n";

        cout << "Dobara search? (y/n): ";
        cin >> again;
    } while (again == 'y' || again == 'Y');

    return 0;
}
