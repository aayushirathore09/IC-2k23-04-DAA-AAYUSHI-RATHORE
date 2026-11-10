/*
 * Program : General Backtracking Technique (template demonstration)
 * Unit    : 6 - Backtracking
 *
 * Problem : Generate all binary strings of length n that contain NO two
 *           consecutive 1s.
 *
 * Backtracking template used in this program:
 *      1. CHOOSE   - extend the partial solution with one option
 *      2. CHECK    - prune immediately if the constraint is violated
 *      3. EXPLORE  - recurse to fill the next position
 *      4. UN-CHOOSE- remove the option and try the next one (the "back" in backtracking)
 *
 * Search space : 2^n strings in total. Pruning cuts the tree early, so only
 *                valid prefixes are ever expanded.
 * Time         : O(number of valid prefixes), far less than O(2^n) nodes of the full tree.
 * Space        : O(n) recursion depth.
 */
#include <iostream>
#include <string>
#include <limits>
using namespace std;

// Reads an integer and keeps asking until the input is valid.
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

// Constraint check: a '1' is not allowed directly after another '1'.
bool isValidChoice(const string &current, char choice) {
    return !(choice == '1' && !current.empty() && current.back() == '1');
}

// 'nodes' counts every recursive call (every node of the search tree that is visited).
void backtrack(int n, string &current, long long &validCount, long long &nodes, bool printResults) {
    nodes++;

    // Base case: a complete string has been built.
    if ((int)current.size() == n) {
        validCount++;
        if (printResults) cout << "  " << current << "\n";
        return;
    }

    for (char choice : {'0', '1'}) {
        if (isValidChoice(current, choice)) {   // CHECK (pruning)
            current.push_back(choice);          // CHOOSE
            backtrack(n, current, validCount, nodes, printResults);  // EXPLORE
            current.pop_back();                 // UN-CHOOSE (backtrack)
        }
    }
}

int main() {
    int n = readInt("Enter string length n (1 to 25): ");
    while (n < 1 || n > 25) n = readInt("Invalid! Enter a value between 1 and 25: ");

    string current;
    long long validCount = 0, nodes = 0;
    bool printResults = (n <= 5);   // print the strings only when the list is short

    if (printResults) cout << "Valid strings:\n";
    backtrack(n, current, validCount, nodes, printResults);

    long long fullTreeNodes = (1LL << (n + 1)) - 1;   // nodes in the complete binary tree
    cout << "\nValid strings (no two consecutive 1s) = " << validCount << "\n";
    cout << "Nodes visited with backtracking        = " << nodes << "\n";
    cout << "Nodes in the full tree (no pruning)    = " << fullTreeNodes << "\n";
    return 0;
}
