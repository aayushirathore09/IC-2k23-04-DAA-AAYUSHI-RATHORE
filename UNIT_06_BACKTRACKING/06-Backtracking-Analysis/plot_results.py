"""
Draws graphs from backtracking_analysis.csv (created by backtracking_analysis.cpp).

Usage:  python plot_results.py
Needs:  pip install matplotlib pandas
Output: backtracking_vs_exhaustive.png
"""
import pandas as pd
import matplotlib.pyplot as plt

data = pd.read_csv("backtracking_analysis.csv")
exhaustive = data.dropna(subset=["exhaustive_ms"])   # exhaustive search only ran for small N

fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(12, 4.5))

# Graph 1: execution time (log scale)
ax1.plot(data["N"], data["backtracking_ms"], marker="o", label="Backtracking")
ax1.plot(exhaustive["N"], exhaustive["exhaustive_ms"], marker="s", label="Exhaustive search")
ax1.set_yscale("log")
ax1.set_xlabel("N (board size)")
ax1.set_ylabel("Execution time (ms, log scale)")
ax1.set_title("N-Queens: execution time")
ax1.grid(True, which="both", alpha=0.3)
ax1.legend()

# Graph 2: search space vs nodes actually explored (log scale)
ax2.plot(data["N"], data["search_space_N_pow_N"], marker="s", label="Search space N^N")
ax2.plot(data["N"], data["backtracking_nodes"], marker="o", label="Nodes explored by backtracking")
ax2.set_yscale("log")
ax2.set_xlabel("N (board size)")
ax2.set_ylabel("Number of states (log scale)")
ax2.set_title("N-Queens: search space vs explored nodes")
ax2.grid(True, which="both", alpha=0.3)
ax2.legend()

plt.tight_layout()
plt.savefig("backtracking_vs_exhaustive.png", dpi=150)
print("Saved backtracking_vs_exhaustive.png")
