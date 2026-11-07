/*
 * Program : Basic Array Operations (Insert, Delete, Search, Update, Traverse)
 * Unit    : 1 - Introduction to Algorithms
 * Note    : Index 0-based hai (pehla element = index 0)
 */
#include <iostream>
#include <string>
#include <limits>
using namespace std;

const int MAX_SIZE = 100;

// Safe integer input (galat input pe dobara puchta hai)
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

// Traverse: saare elements print karo -> O(n)
void traverse(const int arr[], int size) {
    if (size == 0) {
        cout << "Array khali hai.\n";
        return;
    }
    cout << "Array: ";
    for (int i = 0; i < size; i++) cout << arr[i] << " ";
    cout << "\n";
}

// Insert: index 'pos' pe value daalo, aage ke elements right shift -> O(n)
bool insertAt(int arr[], int &size, int pos, int value) {
    if (size >= MAX_SIZE) { cout << "Error: Array full hai.\n"; return false; }
    if (pos < 0 || pos > size) { cout << "Error: Invalid position (0 se " << size << " tak).\n"; return false; }
    for (int i = size; i > pos; i--) arr[i] = arr[i - 1];
    arr[pos] = value;
    size++;
    return true;
}

// Delete: index 'pos' ka element hatao, aage ke elements left shift -> O(n)
bool deleteAt(int arr[], int &size, int pos) {
    if (size == 0) { cout << "Error: Array khali hai.\n"; return false; }
    if (pos < 0 || pos >= size) { cout << "Error: Invalid position (0 se " << size - 1 << " tak).\n"; return false; }
    for (int i = pos; i < size - 1; i++) arr[i] = arr[i + 1];
    size--;
    return true;
}

// Linear Search: index return karta hai, nahi mila to -1 -> O(n)
int linearSearch(const int arr[], int size, int key) {
    for (int i = 0; i < size; i++)
        if (arr[i] == key) return i;
    return -1;
}

// Update: index 'pos' ki value badlo -> O(1)
bool updateAt(int arr[], int size, int pos, int value) {
    if (pos < 0 || pos >= size) { cout << "Error: Invalid position.\n"; return false; }
    arr[pos] = value;
    return true;
}

int main() {
    int arr[MAX_SIZE];
    int size = 0;

    int n = readInt("Shuru me kitne elements daalne hain (0 se " + to_string(MAX_SIZE) + "): ");
    while (n < 0 || n > MAX_SIZE) n = readInt("Invalid size! Dobara dalo: ");
    for (int i = 0; i < n; i++) arr[size++] = readInt("Element " + to_string(i) + ": ");

    int choice;
    do {
        cout << "\n===== ARRAY OPERATIONS =====\n"
             << "1. Traverse\n2. Insert\n3. Delete\n4. Search\n5. Update\n0. Exit\n";
        choice = readInt("Choice: ");

        switch (choice) {
            case 1:
                traverse(arr, size);
                break;
            case 2: {
                int pos = readInt("Position (index): ");
                int val = readInt("Value: ");
                if (insertAt(arr, size, pos, val)) { cout << "Inserted. "; traverse(arr, size); }
                break;
            }
            case 3: {
                int pos = readInt("Position (index): ");
                if (deleteAt(arr, size, pos)) { cout << "Deleted. "; traverse(arr, size); }
                break;
            }
            case 4: {
                int key = readInt("Search value: ");
                int idx = linearSearch(arr, size, key);
                if (idx == -1) cout << key << " array me nahi mila.\n";
                else cout << key << " index " << idx << " pe mila.\n";
                break;
            }
            case 5: {
                int pos = readInt("Position (index): ");
                int val = readInt("New value: ");
                if (updateAt(arr, size, pos, val)) { cout << "Updated. "; traverse(arr, size); }
                break;
            }
            case 0:
                cout << "Exit.\n";
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}
