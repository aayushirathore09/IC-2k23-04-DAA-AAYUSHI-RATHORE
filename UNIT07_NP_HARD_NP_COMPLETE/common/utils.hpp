/*
 * File : utils.hpp
 * Unit : 7 - NP-Hard / NP-Complete
 
 */
#pragma once
#include <chrono>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>
using namespace std;

// ---------------------------------------------------------------- Input
inline int readInt(const string &prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        if (cin.eof()) {                       
            cout << "\nInput khatam ho gaya, program band ho raha hai.\n";
            exit(0);
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Integer dalo: ";
    }
    return value;
}

inline int readIntInRange(const string &prompt, int minVal, int maxVal) {
    int v = readInt(prompt);
    while (v < minVal || v > maxVal)
        v = readInt("Invalid! " + to_string(minVal) + " se " + to_string(maxVal) + " ke beech dalo: ");
    return v;
}

// ---------------------------------------------------------------- Timer
// Wall-clock timer (microseconds me result, double)
class Timer {
    chrono::steady_clock::time_point start_;
public:
    Timer() : start_(chrono::steady_clock::now()) {}
    void reset() { start_ = chrono::steady_clock::now(); }
    double elapsedMicros() const {
        return chrono::duration<double, micro>(chrono::steady_clock::now() - start_).count();
    }
};


template <typename Func>
double averageMicros(Func f, int reps) {
    Timer t;
    for (int i = 0; i < reps; i++) f();
    return t.elapsedMicros() / reps;
}

// ---------------------------------------------------------------- Random
inline mt19937 &rng() {
    static mt19937 gen(12345);          // fixed seed -> experiment reproducible
    return gen;
}
inline void seedRng(unsigned s) { rng().seed(s); }
inline int randInt(int lo, int hi) { return uniform_int_distribution<int>(lo, hi)(rng()); }

// Random TSP instance: n cities 100x100 grid pe, cost = Euclidean distance (rounded, >= 1)
inline vector<vector<int>> randomTspInstance(int n) {
    vector<int> x(n), y(n);
    for (int i = 0; i < n; i++) { x[i] = randInt(0, 100); y[i] = randInt(0, 100); }
    vector<vector<int>> cost(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            if (i != j) {
                double d = sqrt(double(x[i] - x[j]) * (x[i] - x[j]) + double(y[i] - y[j]) * (y[i] - y[j]));
                cost[i][j] = max(1, (int)lround(d));
            }
    return cost;
}

// ---------------------------------------------------------------- Format
inline string humanBytes(long long bytes) {
    const char *unit[] = {"B", "KB", "MB", "GB"};
    double v = (double)bytes;
    int u = 0;
    while (v >= 1024.0 && u < 3) { v /= 1024.0; u++; }
    char buf[32];
    snprintf(buf, sizeof(buf), "%.1f %s", v, unit[u]);
    return buf;
}

inline string humanMicros(double us) {
    char buf[32];
    if (us < 1000.0)            snprintf(buf, sizeof(buf), "%.1f us", us);
    else if (us < 1000000.0)    snprintf(buf, sizeof(buf), "%.2f ms", us / 1000.0);
    else                        snprintf(buf, sizeof(buf), "%.2f s", us / 1000000.0);
    return buf;
}

// Results folder ensure karna (portable: system call avoid, sirf file open try karte hain)
inline ofstream openCsv(const string &path, const string &header) {
    ofstream out(path);
    if (!out) {
        cerr << "CSV file nahi khul payi: " << path
             << "  (pehle 'results' folder banao: mkdir results)\n";
        exit(1);
    }
    out << header << "\n";
    return out;
}

template <typename Func>
double measureMicros(Func f) {
    Timer t;
    f();
    double first = t.elapsedMicros();
    if (first >= 2000.0) return first;
    int reps = (int)min(200.0, 20000.0 / max(first, 1.0)) + 1;
    return averageMicros(f, reps);
}
