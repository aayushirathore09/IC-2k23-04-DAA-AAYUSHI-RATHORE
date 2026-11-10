"""
Unit 5 | Experiment 5: Performance analysis of BFS vs DFS and
adjacency matrix vs adjacency list.

What it does
  1. Generates random connected graphs of increasing size (fixed seed).
  2. Measures BFS / DFS time on both representations (median of REPEATS runs).
  3. Measures memory used to store each representation (tracemalloc).
  4. Verifies every run: all V vertices visited, list result == matrix result.
  5. Stores everything in results.csv and draws graphs (PNG).
  6. Compares theoretical operation counts with measured time (linear fit, R^2).

Usage:  python performance_analysis.py          (needs matplotlib, numpy)
"""
import csv
import os
import random
import statistics
import sys
import time
import tracemalloc

import matplotlib
matplotlib.use("Agg")                      # draw to files, no window needed
import matplotlib.pyplot as plt
import numpy as np

HERE = os.path.dirname(os.path.abspath(__file__))
for folder in ("02-Graph-Representation", "03-BFS", "04-DFS"):
    sys.path.insert(0, os.path.join(HERE, "..", folder))
from graph_representation import AdjacencyList, AdjacencyMatrix  # noqa: E402
from bfs import bfs                                               # noqa: E402
from dfs import dfs_iterative                                     # noqa: E402

SEED = 42
REPEATS = 5
SPARSE_SIZES = list(range(200, 2001, 200))     # E = 3V
DENSE_SIZES = list(range(100, 801, 100))       # E = V(V-1)/4  (half-dense)


def random_connected_graph(n, m, rng):
    """Random connected undirected graph with n vertices and m edges (n-1 <= m <= n(n-1)/2)."""
    max_edges = n * (n - 1) // 2
    if not n - 1 <= m <= max_edges:
        raise ValueError(f"m must be between {n - 1} and {max_edges}")
    edges = set()
    for v in range(1, n):                      # random spanning tree -> connected
        u = rng.randrange(v)
        edges.add((u, v))
    while len(edges) < m:                      # add random extra edges
        u, v = rng.sample(range(n), 2)
        edges.add((min(u, v), max(u, v)))
    return sorted(edges)


def median_time_ms(function, *args):
    samples = []
    for _ in range(REPEATS):
        start = time.perf_counter()
        function(*args)
        samples.append((time.perf_counter() - start) * 1000)
    return statistics.median(samples)


def measure_memory_kb(cls, n, edges):
    """Memory used by one representation (in KB), edge list excluded."""
    tracemalloc.start()
    graph = cls.from_edges(n, edges)
    current, _ = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    return graph, current / 1024


def run_experiment(label, sizes, edge_rule, rng):
    rows = []
    for n in sizes:
        m = edge_rule(n)
        edges = random_connected_graph(n, m, rng)
        g_list, mem_list = measure_memory_kb(AdjacencyList, n, edges)
        g_mat, mem_mat = measure_memory_kb(AdjacencyMatrix, n, edges)

        # Verification of correctness
        b_list, b_mat = bfs(g_list, 0), bfs(g_mat, 0)
        d_list, d_mat = dfs_iterative(g_list, 0), dfs_iterative(g_mat, 0)
        assert b_list == b_mat and d_list == d_mat, "list and matrix results differ"
        assert len(b_list["order"]) == n and len(d_list["order"]) == n, "graph not fully explored"

        row = {
            "experiment": label, "V": n, "E": m,
            "bfs_list_ms": median_time_ms(bfs, g_list, 0),
            "bfs_matrix_ms": median_time_ms(bfs, g_mat, 0),
            "dfs_list_ms": median_time_ms(dfs_iterative, g_list, 0),
            "dfs_matrix_ms": median_time_ms(dfs_iterative, g_mat, 0),
            "nodes_visited": len(b_list["order"]),
            "ops_list_V_plus_2E": n + 2 * m,
            "ops_matrix_V2": n * n,
            "mem_list_kb": mem_list, "mem_matrix_kb": mem_mat,
        }
        rows.append(row)
        print(f"[{label:6}] V={n:5d} E={m:7d} | BFS list {row['bfs_list_ms']:8.3f} ms "
              f"matrix {row['bfs_matrix_ms']:8.3f} ms | DFS list {row['dfs_list_ms']:8.3f} ms "
              f"matrix {row['dfs_matrix_ms']:8.3f} ms")
    return rows


def save_csv(rows, path):
    with open(path, "w", newline="") as handle:
        writer = csv.DictWriter(handle, fieldnames=list(rows[0].keys()))
        writer.writeheader()
        for row in rows:
            writer.writerow({k: (round(v, 4) if isinstance(v, float) else v) for k, v in row.items()})


def column(rows, key):
    return [r[key] for r in rows]


def plot_time(rows, title, path):
    x = column(rows, "V")
    plt.figure(figsize=(7, 4.5))
    plt.plot(x, column(rows, "bfs_list_ms"), "o-", label="BFS - adjacency list")
    plt.plot(x, column(rows, "bfs_matrix_ms"), "s-", label="BFS - adjacency matrix")
    plt.plot(x, column(rows, "dfs_list_ms"), "^--", label="DFS - adjacency list")
    plt.plot(x, column(rows, "dfs_matrix_ms"), "d--", label="DFS - adjacency matrix")
    plt.xlabel("Number of vertices V")
    plt.ylabel("Execution time (ms, median of 5 runs)")
    plt.title(title)
    plt.grid(alpha=0.3)
    plt.legend()
    plt.tight_layout()
    plt.savefig(path, dpi=150)
    plt.close()


def plot_memory(sparse, dense, path):
    fig, axes = plt.subplots(1, 2, figsize=(11, 4.2))
    for ax, rows, name in ((axes[0], sparse, "Sparse graphs (E = 3V)"),
                           (axes[1], dense, "Dense graphs (E = V(V-1)/4)")):
        ax.plot(column(rows, "V"), column(rows, "mem_list_kb"), "o-", label="Adjacency list")
        ax.plot(column(rows, "V"), column(rows, "mem_matrix_kb"), "s-", label="Adjacency matrix")
        ax.set_xlabel("Number of vertices V")
        ax.set_ylabel("Memory (KB)")
        ax.set_title(name)
        ax.grid(alpha=0.3)
        ax.legend()
    plt.tight_layout()
    plt.savefig(path, dpi=150)
    plt.close()


def fit_line(ops, times):
    """Least-squares line time = a*ops + b ; returns (a, b, R^2)."""
    ops, times = np.array(ops, float), np.array(times, float)
    a, b = np.polyfit(ops, times, 1)
    predicted = a * ops + b
    ss_res = float(np.sum((times - predicted) ** 2))
    ss_tot = float(np.sum((times - times.mean()) ** 2))
    return a, b, 1 - ss_res / ss_tot


def plot_theory_vs_empirical(rows, path):
    """Measured BFS time against the theoretical operation count."""
    fig, axes = plt.subplots(1, 2, figsize=(11, 4.2))
    summary = []
    for ax, ops_key, time_key, title in (
            (axes[0], "ops_list_V_plus_2E", "bfs_list_ms", "Adjacency list: time vs (V + 2E)"),
            (axes[1], "ops_matrix_V2", "bfs_matrix_ms", "Adjacency matrix: time vs V^2")):
        ops, times = column(rows, ops_key), column(rows, time_key)
        a, b, r2 = fit_line(ops, times)
        ax.scatter(ops, times, label="measured")
        xs = np.linspace(min(ops), max(ops), 100)
        ax.plot(xs, a * xs + b, "r-", label=f"linear fit  (R^2 = {r2:.4f})")
        ax.set_xlabel("Theoretical operations")
        ax.set_ylabel("BFS time (ms)")
        ax.set_title(title)
        ax.grid(alpha=0.3)
        ax.legend()
        summary.append((title, r2))
    plt.tight_layout()
    plt.savefig(path, dpi=150)
    plt.close()
    return summary


def main():
    rng = random.Random(SEED)
    print("Running sparse-graph experiment ...")
    sparse = run_experiment("sparse", SPARSE_SIZES, lambda n: 3 * n, rng)
    print("\nRunning dense-graph experiment ...")
    dense = run_experiment("dense", DENSE_SIZES, lambda n: n * (n - 1) // 4, rng)

    all_rows = sparse + dense
    save_csv(all_rows, os.path.join(HERE, "results.csv"))
    plot_time(sparse, "BFS / DFS time - sparse graphs (E = 3V)", os.path.join(HERE, "time_sparse.png"))
    plot_time(dense, "BFS / DFS time - dense graphs (E = V(V-1)/4)", os.path.join(HERE, "time_dense.png"))
    plot_memory(sparse, dense, os.path.join(HERE, "memory.png"))
    summary = plot_theory_vs_empirical(all_rows, os.path.join(HERE, "theory_vs_empirical.png"))

    print("\nTheoretical vs empirical complexity (linear fit of BFS time against operation count):")
    for title, r2 in summary:
        print(f"  {title:40s} R^2 = {r2:.4f}")
    print("\nSaved: results.csv, time_sparse.png, time_dense.png, memory.png, theory_vs_empirical.png")


if __name__ == "__main__":
    main()
