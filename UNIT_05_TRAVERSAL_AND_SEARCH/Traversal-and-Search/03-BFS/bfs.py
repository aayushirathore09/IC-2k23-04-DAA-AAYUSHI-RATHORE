"""
Unit 5 | Experiment 3: Breadth First Search (BFS).

Works on ANY graph object from graph_representation.py (list or matrix)
because it only uses graph.n and graph.neighbors(u).

  * bfs()                  : visiting order, distance (edges) and parent of every vertex
  * shortest_path()        : fewest-edge path using the parent array
  * connected_components() : components of an undirected graph

Complexity: adjacency list  O(V + E)     adjacency matrix  O(V^2)     Space O(V)

Usage:
    python bfs.py --demo            # sample graph, adjacency list
    python bfs.py --demo --matrix   # same graph, adjacency matrix
    python bfs.py                   # interactive
"""
import os
import sys
from collections import deque

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                "..", "02-Graph-Representation"))
from graph_representation import get_graph_from_cli, read_int  # noqa: E402


def bfs(graph, source):
    """Breadth-first search from `source`.

    Returns dict(order, dist, parent); dist/parent are -1 for unreachable vertices.
    """
    graph.validate_vertex(source)
    visited = [False] * graph.n
    dist = [-1] * graph.n
    parent = [-1] * graph.n
    order = []

    visited[source] = True              # mark when ENQUEUED (avoids duplicates)
    dist[source] = 0
    queue = deque([source])
    while queue:
        u = queue.popleft()
        order.append(u)
        for v in graph.neighbors(u):
            if not visited[v]:
                visited[v] = True
                dist[v] = dist[u] + 1
                parent[v] = u
                queue.append(v)
    return {"order": order, "dist": dist, "parent": parent}


def shortest_path(result, source, target):
    """Rebuild the fewest-edge path source -> target, or None if unreachable."""
    if result["dist"][target] == -1:
        return None
    path, node = [], target
    while node != -1:
        path.append(node)
        node = result["parent"][node]
    return path[::-1]


def levels(result):
    """Group vertices by BFS level (distance from the source)."""
    grouped = {}
    for v, d in enumerate(result["dist"]):
        if d != -1:
            grouped.setdefault(d, []).append(v)
    return grouped


def connected_components(graph):
    """All connected components (undirected graph) using repeated BFS. O(V + E)."""
    visited = [False] * graph.n
    components = []
    for start in range(graph.n):
        if visited[start]:
            continue
        visited[start] = True
        queue, component = deque([start]), []
        while queue:
            u = queue.popleft()
            component.append(u)
            for v in graph.neighbors(u):
                if not visited[v]:
                    visited[v] = True
                    queue.append(v)
        components.append(component)
    return components


def main():
    graph = get_graph_from_cli()
    kind = type(graph).__name__
    print(f"\nGraph: V = {graph.n}, E = {graph.edge_count} ({kind})")

    if "--demo" in sys.argv:
        source, target = 0, 7
    else:
        source = read_int(f"Source vertex (0..{graph.n - 1}): ", 0, graph.n - 1)
        target = read_int(f"Target vertex for shortest path (0..{graph.n - 1}): ", 0, graph.n - 1)

    result = bfs(graph, source)
    print(f"\nBFS from vertex {source}")
    print(f"  Visiting order : {result['order']}")
    print("  Levels         :")
    for d, vs in sorted(levels(result).items()):
        print(f"     level {d}: {vs}")
    print(f"  Distances      : {result['dist']}   (-1 = unreachable)")
    print(f"  Parents        : {result['parent']}")

    path = shortest_path(result, source, target)
    if path is None:
        print(f"\nNo path from {source} to {target}.")
    else:
        print(f"\nShortest path {source} -> {target}: {' -> '.join(map(str, path))}"
              f"  ({len(path) - 1} edges)")

    unreachable = [v for v in range(graph.n) if result["dist"][v] == -1]
    print(f"Unreachable from {source}: {unreachable if unreachable else 'none'}")
    print(f"Connected components: {connected_components(graph)}")


if __name__ == "__main__":
    main()
