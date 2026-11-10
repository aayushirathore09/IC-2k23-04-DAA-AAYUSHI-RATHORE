/*
 * Program : Constraint Satisfaction Solver (Application) - Backtracking
 * Unit    : 6 - Backtracking
 *
 * A Constraint Satisfaction Problem (CSP) has:
 *     - VARIABLES    (what we have to assign),
 *     - DOMAINS      (the allowed values of each variable),
 *     - CONSTRAINTS  (rules the assignment must obey).
 * Backtracking assigns one variable at a time and undoes the assignment as soon as a
 * constraint is violated.
 *
 * This solver handles two classic CSPs:
 *   1. Graph / Map Coloring : variables = vertices, domain = colors 1..k,
 *                             constraint = adjacent vertices need different colors.
 *                             It also finds the MINIMUM number of colors needed.
 *   2. Sudoku               : variables = the 81 cells, domain = digits 1..9,
 *                             constraint = no repeated digit in a row, column or 3x3 box.
 *
 * Time (worst case) : Graph coloring O(k^V), Sudoku O(9^(empty cells)). Pruning makes
 *                     typical inputs fast.
 * Space             : O(V) for coloring, O(1) extra for Sudoku (grid is modified in place).
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <chrono>
#include <algorithm>
using namespace std;

int readInt(const string &prompt) {
    int value;
    cout << prompt;
    while (!(cin >> value)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input! Please enter an integer: ";
    }
    return value;
}

int readIntInRange(const string &prompt, int minVal, int maxVal) {
    int v = readInt(prompt);
    while (v < minVal || v > maxVal)
        v = readInt("Invalid! Enter a value between " + to_string(minVal) + " and " + to_string(maxVal) + ": ");
    return v;
}

// =============================== Graph coloring ===============================

// Constraint: no neighbour of vertex v may already use color c.
bool isColorSafe(const vector<vector<int>> &adj, const vector<int> &color, int v, int c) {
    for (int u : adj[v])
        if (color[u] == c) return false;
    return true;
}

// Tries to color vertices v..V-1 using colors 1..k. 'nodes' counts the color tries.
bool colorGraph(const vector<vector<int>> &adj, int V, int k, int v, vector<int> &color, long long &nodes) {
    if (v == V) return true;                       // every vertex is colored
    for (int c = 1; c <= k; c++) {
        nodes++;
        if (isColorSafe(adj, color, v, c)) {
            color[v] = c;                          // assign
            if (colorGraph(adj, V, k, v + 1, color, nodes)) return true;
            color[v] = 0;                          // undo (backtrack)
        }
    }
    return false;                                  // no color works -> go back to the previous vertex
}

void runGraphColoring() {
    int V = readIntInRange("Number of vertices (1 to 50): ", 1, 50);
    int maxEdges = V * (V - 1) / 2;
    int E = readIntInRange("Number of edges (0 to " + to_string(maxEdges) + "): ", 0, maxEdges);
    int maxColors = readIntInRange("Maximum number of colors to try (1 to 10): ", 1, 10);

    vector<vector<int>> adj(V);
    cout << "Enter each edge as: u v   (vertices are numbered 0 to " << V - 1 << ")\n";
    for (int i = 0; i < E; i++) {
        int u = readInt("Edge " + to_string(i + 1) + " - u: ");
        int v = readInt("Edge " + to_string(i + 1) + " - v: ");
        if (u < 0 || u >= V || v < 0 || v >= V || u == v) {
            cout << "Invalid edge! Please enter it again.\n";
            i--;
            continue;
        }
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    long long nodes = 0;
    vector<int> color(V, 0);
    auto start = chrono::high_resolution_clock::now();

    // Try k = 1, 2, 3 ... the first k that works is the minimum number of colors (chromatic number).
    int usedColors = -1;
    for (int k = 1; k <= maxColors; k++) {
        fill(color.begin(), color.end(), 0);
        if (colorGraph(adj, V, k, 0, color, nodes)) { usedColors = k; break; }
    }
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();

    if (usedColors == -1) {
        cout << "\nThe graph cannot be colored with " << maxColors << " colors or fewer.\n";
    } else {
        cout << "\nMinimum number of colors needed = " << usedColors << "\n";
        for (int v = 0; v < V; v++) cout << "  Vertex " << v << " -> Color " << color[v] << "\n";
    }
    cout << "Color assignments tried = " << nodes << ", time = " << ms << " ms\n";
}

// ================================== Sudoku ==================================

// Constraint: 'num' must not already appear in the row, the column or the 3x3 box.
bool isSudokuSafe(const vector<vector<int>> &grid, int row, int col, int num) {
    for (int i = 0; i < 9; i++) {
        if (grid[row][i] == num) return false;
        if (grid[i][col] == num) return false;
    }
    int boxRow = (row / 3) * 3, boxCol = (col / 3) * 3;
    for (int r = 0; r < 3; r++)
        for (int c = 0; c < 3; c++)
            if (grid[boxRow + r][boxCol + c] == num) return false;
    return true;
}

bool solveSudoku(vector<vector<int>> &grid, long long &nodes) {
    // Find the next empty cell (value 0).
    int row = -1, col = -1;
    for (int r = 0; r < 9 && row == -1; r++)
        for (int c = 0; c < 9; c++)
            if (grid[r][c] == 0) { row = r; col = c; break; }

    if (row == -1) return true;                    // no empty cell left -> solved

    for (int num = 1; num <= 9; num++) {
        nodes++;
        if (isSudokuSafe(grid, row, col, num)) {
            grid[row][col] = num;                  // place digit
            if (solveSudoku(grid, nodes)) return true;
            grid[row][col] = 0;                    // undo (backtrack)
        }
    }
    return false;
}

// Checks that the given clues do not already break a rule.
bool isInitialGridValid(vector<vector<int>> &grid) {
    for (int r = 0; r < 9; r++)
        for (int c = 0; c < 9; c++) {
            int num = grid[r][c];
            if (num == 0) continue;
            grid[r][c] = 0;                        // temporarily remove it to test against the others
            bool ok = isSudokuSafe(grid, r, c, num);
            grid[r][c] = num;
            if (!ok) return false;
        }
    return true;
}

void printGrid(const vector<vector<int>> &grid) {
    for (int r = 0; r < 9; r++) {
        if (r % 3 == 0 && r != 0) cout << "------+-------+------\n";
        for (int c = 0; c < 9; c++) {
            if (c % 3 == 0 && c != 0) cout << "| ";
            cout << grid[r][c] << " ";
        }
        cout << "\n";
    }
}

void runSudoku() {
    cout << "Enter the puzzle as 9 rows of 9 digits each (use 0 for an empty cell).\n";
    vector<vector<int>> grid(9, vector<int>(9, 0));
    for (int r = 0; r < 9; r++) {
        string line;
        while (true) {
            cout << "Row " << r + 1 << ": ";
            cin >> line;
            bool valid = (line.size() == 9);
            for (char ch : line) if (ch < '0' || ch > '9') valid = false;
            if (valid) break;
            cout << "Invalid row! Enter exactly 9 digits (0-9).\n";
        }
        for (int c = 0; c < 9; c++) grid[r][c] = line[c] - '0';
    }

    if (!isInitialGridValid(grid)) {
        cout << "\nThe puzzle is invalid: a digit is repeated in a row, column or box.\n";
        return;
    }

    long long nodes = 0;
    auto start = chrono::high_resolution_clock::now();
    bool solved = solveSudoku(grid, nodes);
    auto end = chrono::high_resolution_clock::now();
    double ms = chrono::duration<double, milli>(end - start).count();

    if (solved) {
        cout << "\nSolved Sudoku:\n";
        printGrid(grid);
    } else {
        cout << "\nThis puzzle has no solution.\n";
    }
    cout << "Digit placements tried = " << nodes << ", time = " << ms << " ms\n";
}

int main() {
    int choice;
    do {
        cout << "\n===== CONSTRAINT SATISFACTION SOLVER =====\n"
             << "1. Graph / Map Coloring\n2. Sudoku\n0. Exit\n";
        choice = readInt("Choice: ");
        if (choice == 1) runGraphColoring();
        else if (choice == 2) runSudoku();
        else if (choice != 0) cout << "Invalid choice!\n";
    } while (choice != 0);
    return 0;
}
