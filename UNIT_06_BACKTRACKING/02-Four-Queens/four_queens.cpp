/*
 * Program : 4-Queens Problem using Backtracking
 * Unit    : 6 - Backtracking
 *
 * Problem : Place 4 queens on a 4x4 board so that no two queens attack each other
 *           (no two share a row, column or diagonal).
 *
 * Representation: col[r] = column of the queen placed in row r.
 *                 One queen per row is guaranteed by construction, so only
 *                 columns and diagonals have to be checked.
 *
 * Time    : O(N!) worst case, much smaller in practice because of pruning.
 * Space   : O(N) for the placement array and recursion stack.
 */
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

const int N = 4;

// Can a queen go at (row, c)? Compare with every queen already placed in rows 0..row-1.
bool isSafe(const vector<int> &col, int row, int c) {
    for (int prev = 0; prev < row; prev++) {
        if (col[prev] == c) return false;                        // same column
        if (abs(col[prev] - c) == row - prev) return false;      // same diagonal
    }
    return true;
}

void printBoard(const vector<int> &col, int solutionNo) {
    cout << "Solution " << solutionNo << ":\n";
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) cout << (col[r] == c ? "Q " : ". ");
        cout << "\n";
    }
    cout << "\n";
}

// Place queens row by row; 'count' is the number of solutions found so far.
void solve(int row, vector<int> &col, int &count) {
    if (row == N) {                       // all N queens placed
        printBoard(col, ++count);
        return;
    }
    for (int c = 0; c < N; c++) {
        if (isSafe(col, row, c)) {        // prune unsafe squares
            col[row] = c;                 // place queen
            solve(row + 1, col, count);   // go to the next row
            col[row] = -1;                // remove queen (backtrack)
        }
    }
}

int main() {
    vector<int> col(N, -1);
    int count = 0;
    solve(0, col, count);
    cout << "Total solutions for " << N << "-Queens = " << count << "\n";
    return 0;
}
