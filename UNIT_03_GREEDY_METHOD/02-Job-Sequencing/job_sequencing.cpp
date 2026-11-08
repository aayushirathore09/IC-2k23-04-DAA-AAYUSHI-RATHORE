/*
 * Program : Job Sequencing with Deadlines (Greedy)
 * Unit    : 3 - Greedy Method
 *
 * job shoul complete befor deadline.
 * Greedy choice : job sort in descending order.
 *                 every job given slot before its deadline , an empy slot.
 * Time          : O(n log n) sorting + O(n * maxDeadline) slot search  ~ O(n^2) worst
 * Space         : O(n)
 */
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <algorithm>
using namespace std;

struct Job {
    int id;
    int deadline;
    int profit;
};

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

int readIntInRange(const string &prompt, int minVal, int maxVal) {
    int v = readInt(prompt);
    while (v < minVal || v > maxVal)
        v = readInt("Invalid! " + to_string(minVal) + " se " + to_string(maxVal) + " ke beech dalo: ");
    return v;
}

// slots[t] = us time slot (1..maxDeadline) me kaunsi job hai (0 = khali)
// Return: total profit
int jobSequencing(vector<Job> jobs, int maxDeadline, vector<int> &slots) {
    sort(jobs.begin(), jobs.end(),
         [](const Job &a, const Job &b) { return a.profit > b.profit; });

    slots.assign(maxDeadline + 1, 0);
    int totalProfit = 0;

    for (const Job &job : jobs) {
        // find an empty slot,
        for (int t = job.deadline; t >= 1; t--) {
            if (slots[t] == 0) {
                slots[t] = job.id;
                totalProfit += job.profit;
                break;
            }
        }
        // slot not found -> job skip
    }
    return totalProfit;
}

int main() {
    int n = readIntInRange("Jobs ki sankhya (1 se 1000): ", 1, 1000);
    vector<Job> jobs(n);
    int maxDeadline = 0;

    for (int i = 0; i < n; i++) {
        jobs[i].id = i + 1;
        jobs[i].deadline = readIntInRange("Job " + to_string(i + 1) + " deadline (>=1): ", 1, 1000000);
        jobs[i].profit   = readIntInRange("Job " + to_string(i + 1) + " profit   (>=0): ", 0, 1000000000);
        maxDeadline = max(maxDeadline, jobs[i].deadline);
    }
    
    maxDeadline = min(maxDeadline, n);
    for (Job &j : jobs) j.deadline = min(j.deadline, maxDeadline);

    vector<int> slots;
    int total = jobSequencing(jobs, maxDeadline, slots);

    cout << "\nSchedule (time slot -> job):\n";
    int scheduled = 0;
    for (int t = 1; t <= maxDeadline; t++) {
        if (slots[t] != 0) { cout << "  Slot " << t << ": Job " << slots[t] << "\n"; scheduled++; }
        else               { cout << "  Slot " << t << ": (khali)\n"; }
    }
    cout << "Jobs scheduled = " << scheduled << " / " << n << "\n";
    cout << "Maximum total profit = " << total << "\n";
    return 0;
}
