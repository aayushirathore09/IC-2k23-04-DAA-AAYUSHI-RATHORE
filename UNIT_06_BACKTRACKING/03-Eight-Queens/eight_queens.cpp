/*
 * Program : 8-Queens Problem using Backtracking
 * Unit    : 6 - Backtracking
 *
 * Problem : Place 8 queens on a standard 8x8 chessboard so that no two queens attack
 *           each other. The program counts all solutions (the answer is 92) and shows
 *           the first one. All solutions can optionally be listed.
 *
 * Representation: col[r] = column of the queen in row r.
 *
 * Time    : O(N!) worst case, with strong pruning in practice.
 * Space   : O(N)
 */
#include <iostream>
#include <vector>
#include <cstdlib>
#include <string>
using namespace std;

const int N = 8;

bool isSafe(const vector<int> &col, int row, int c) {
    for (int prev = 0; prev < row; prev++) {
        if (col[prev] == c) return false;                        // same column
        if (abs(col[prev] - c) == row - prev) return false;      // same diagonal
    }
    return true;
}

void printBoard(const vector<int> &col) {
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) cout << (col[r] == c ? "Q " : ". ");
        cout << "\n";
    }
}

// Prints a solution on one line, e.g. "1: 0 4 7 5 2 6 1 3" (column of the queen in each row).
void printCompact(const vector<int> &col, int number) {
    cout << number << ": ";
    for (int r = 0; r < N; r++) cout << col[r] << " ";
    cout << "\n";
}

void solve(int row, vector<int> &col, int &count, bool listAll) {
    if (row == N) {
        count++;
        if (count == 1) {                         // always show the first solution as a board
            cout << "First solution:\n";
            printBoard(col);
            cout << "\n";
            if (listAll) cout << "All solutions (column of the queen in each row):\n";
        }
        if (listAll) printCompact(col, count);
        return;
    }
    for (int c = 0; c < N; c++) {
        if (isSafe(col, row, c)) {
            col[row] = c;                         // place queen
            solve(row + 1, col, count, listAll);
            col[row] = -1;                        // backtrack
        }
    }
}

int main() {
    char answer;
    cout << "List all 92 solutions? (y/n): ";
    cin >> answer;
    bool listAll = (answer == 'y' || answer == 'Y');

    vector<int> col(N, -1);
    int count = 0;
    solve(0, col, count, listAll);
    cout << "\nTotal solutions for " << N << "-Queens = " << count << "\n";
    return 0;
}
