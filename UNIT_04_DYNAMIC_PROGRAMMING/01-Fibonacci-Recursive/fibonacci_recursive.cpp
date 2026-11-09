/*
 * Program : Fibonacci using plain Recursion
 * Unit    : 4 - Dynamic Programming (baseline for memoization comparison)
 *
 * fib(0) = 0, fib(1) = 1, fib(n) = fib(n-1) + fib(n-2)
 *
 * Time    : O(2^n)  (same subproblems baar-baar solve hote hain)
 * Space   : O(n)    (recursion stack depth)
 * Limit   : n <= 45 (usse bada n bahut slow ho jata hai)
 */
#include <iostream>
#include <string>
#include <limits>
#include <chrono>
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

// 'calls' me total recursive calls ginte hain
long long fibRecursive(int n, long long &calls) {
    calls++;
    if (n <= 1) return n;                                   // base cases
    return fibRecursive(n - 1, calls) + fibRecursive(n - 2, calls);
}

int main() {
    int n = readInt("n dalo (0 se 45): ");
    while (n < 0 || n > 45) n = readInt("Invalid! 0 se 45 ke beech dalo: ");
    if (n > 40) cout << "Warning: n > 40 pe thoda slow ho sakta hai...\n";

    long long calls = 0;
    auto start = chrono::high_resolution_clock::now();
    long long result = fibRecursive(n, calls);
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();

    cout << "fib(" << n << ") = " << result << "\n";
    cout << "Total recursive calls = " << calls << "\n";
    cout << "Time = " << ms << " ms\n";
    return 0;
}
