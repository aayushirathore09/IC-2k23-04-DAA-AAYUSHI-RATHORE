/*
 * Program : Fibonacci using Memoization (Top-Down Dynamic Programming)
 * Unit    : 4 - Dynamic Programming
 *
 * Idea    : recursion wahi, bas har fib(i) ka result table (memo) me store karo.
 *           Dobara zaroorat pade to calculate karne ki jagah table se utha lo.
 *
 * Time    : O(n)   (har fib(i) sirf ek baar calculate hota hai)
 * Space   : O(n)   (memo table + recursion stack)
 * Limit   : n <= 92 (fib(93) 'long long' me overflow ho jata hai)
 */
#include <iostream>
#include <vector>
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

// memo[i] = fib(i), -1 ka matlab abhi calculate nahi hua
long long fibMemo(int n, vector<long long> &memo, long long &calls) {
    calls++;
    if (n <= 1) return n;                          // base cases
    if (memo[n] != -1) return memo[n];             // pehle se pata hai -> seedha return
    memo[n] = fibMemo(n - 1, memo, calls) + fibMemo(n - 2, memo, calls);
    return memo[n];
}

int main() {
    int n = readInt("n dalo (0 se 92): ");
    while (n < 0 || n > 92) n = readInt("Invalid! 0 se 92 ke beech dalo (long long limit): ");

    vector<long long> memo(n + 1, -1);
    long long calls = 0;

    auto start = chrono::high_resolution_clock::now();
    long long result = fibMemo(n, memo, calls);
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();

    cout << "fib(" << n << ") = " << result << "\n";
    cout << "Total recursive calls = " << calls << "\n";
    cout << "Time = " << ms << " ms\n";
    return 0;
}
