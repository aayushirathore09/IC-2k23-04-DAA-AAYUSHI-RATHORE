/*
 * Program : Iterative Binary Search
 * Unit    : 1 - Introduction to Algorithms
 * Time    : Best O(1), Average/Worst O(log n)
 * Space   : O(1)
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

// Binary Search ke liye array sorted (ascending) hona zaroori hai
bool isSorted(const int arr[], int n) {
    for (int i = 1; i < n; i++)
        if (arr[i - 1] > arr[i]) return false;
    return true;
}

// Iterative Binary Search. 'comparisons' me steps ginte hain.
// Return: key ka index, nahi mila to -1
int binarySearchIterative(const int arr[], int n, int key, int &comparisons) {
    int low = 0, high = n - 1;
    comparisons = 0;
    while (low <= high) {
        int mid = low + (high - low) / 2;   // overflow se bachne ke liye
        comparisons++;
        if (arr[mid] == key) return mid;
        else if (arr[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
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
        int comparisons;
        int idx = binarySearchIterative(arr, n, key, comparisons);

        if (idx == -1) cout << key << " nahi mila. ";
        else cout << key << " index " << idx << " pe mila. ";
        cout << "(Comparisons: " << comparisons << ")\n";

        cout << "Dobara search? (y/n): ";
        cin >> again;
    } while (again == 'y' || again == 'Y');

    return 0;
}
