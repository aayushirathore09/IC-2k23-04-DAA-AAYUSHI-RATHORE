"""
Unit 5 | Experiment 4: Depth First Search (DFS) - recursive AND iterative.

Both versions return the SAME visiting order (the iterative version keeps an
explicit stack of (vertex, neighbour-iterator) pairs, which mimics recursion).
Discovery / finish times are recorded: disc[u] < disc[v] < fin[v] < fin[u]
whenever v is a descendant of u.

Complexity: adjacency list  O(V + E)     adjacency matrix  O(V^2)
Space     : O(V) for visited + recursion/explicit stack

Usage:
    python dfs.py --demo            # sample graph, adjacency list
    python dfs.py --demo --matrix   # adjacency matrix
    python dfs.py                   # interactive
"""
import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)),
                                "..", "02-Graph-Representation"))
from graph_representation import get_graph_from_cli, read_int  # noqa: E402

MAX_RECURSIVE_VERTICES = 20000      # beyond this use dfs_iterative (stack safety)


def dfs_recursive(graph, source):
    """Recursive DFS. Returns dict(order, parent, disc, fin)."""
    graph.validate_vertex(source)
    if graph.n > MAX_RECURSIVE_VERTICES:
        raise ValueError("Graph too large for recursion - use dfs_iterative()")
    if graph.n + 50 > sys.getrecursionlimit():
        sys.setrecursionlimit(graph.n + 50)

    visited = [False] * graph.n
    parent = [-1] * graph.n
    disc = [0] * graph.n
    fin = [0] * graph.n
    order = []
    clock = [0]                              # list so the inner function can modify it

    def visit(u):
        visited[u] = True
        clock[0] += 1
        disc[u] = clock[0]
        order.append(u)
        for v in graph.neighbors(u):
            if not visited[v]:
                parent[v] = u
                visit(v)
        clock[0] += 1
        fin[u] = clock[0]

    visit(source)
    return {"order": order, "parent": parent, "disc": disc, "fin": fin}


def dfs_iterative(graph, source):
    """Iterative DFS using an explicit stack. Same result as dfs_recursive()."""
    graph.validate_vertex(source)
    visited = [False] * graph.n
    parent = [-1] * graph.n
    disc = [0] * graph.n
    fin = [0] * graph.n
    order = []
    clock = 1

    visited[source] = True
    disc[source] = clock
    order.append(source)
    stack = [(source, iter(graph.neighbors(source)))]
    while stack:
        u, neighbours = stack[-1]
        for v in neighbours:                 # resumes where it stopped last time
            if not visited[v]:
                visited[v] = True
                parent[v] = u
                clock += 1
                disc[v] = clock
                order.append(v)
                stack.append((v, iter(graph.neighbors(v))))
                break
        else:                                # no unvisited neighbour left -> backtrack
            clock += 1
            fin[u] = clock
            stack.pop()
    return {"order": order, "parent": parent, "disc": disc, "fin": fin}


def dfs_path(result, source, target):
    """A (not necessarily shortest) path found by DFS, or None."""
    if target != source and result["parent"][target] == -1:
        return None
    path, node = [], target
    while node != -1:
        path.append(node)
        node = result["parent"][node]
    return path[::-1]


def connected_components(graph):
    """Components of an undirected graph with repeated iterative DFS. O(V + E)."""
    visited = [False] * graph.n
    components = []
    for start in range(graph.n):
        if visited[start]:
            continue
        visited[start] = True
        component = [start]
        stack = [(start, iter(graph.neighbors(start)))]
        while stack:
            u, neighbours = stack[-1]
            for v in neighbours:
                if not visited[v]:
                    visited[v] = True
                    component.append(v)
                    stack.append((v, iter(graph.neighbors(v))))
                    break
            else:
                stack.pop()
        components.append(component)
    return components


def main():
    graph = get_graph_from_cli()
    print(f"\nGraph: V = {graph.n}, E = {graph.edge_count} ({type(graph).__name__})")

    if "--demo" in sys.argv:
        source, target = 0, 7
    else:
        source = read_int(f"Source vertex (0..{graph.n - 1}): ", 0, graph.n - 1)
        target = read_int(f"Target vertex for path (0..{graph.n - 1}): ", 0, graph.n - 1)

    rec = dfs_recursive(graph, source)
    itr = dfs_iterative(graph, source)

    print(f"\nDFS from vertex {source}")
    print(f"  Recursive order : {rec['order']}")
    print(f"  Iterative order : {itr['order']}")
    assert rec == itr, "Recursive and iterative DFS disagree!"
    print("  Verification    : recursive == iterative -> OK")

    print("\n  vertex  discovery  finish  parent")
    for v in rec["order"]:
        print(f"  {v:>6}  {rec['disc'][v]:>9}  {rec['fin'][v]:>6}  {rec['parent'][v]:>6}")

    path = dfs_path(rec, source, target)
    print(f"\nPath found by DFS {source} -> {target}: "
          f"{' -> '.join(map(str, path)) if path else 'none'}")
    print(f"Connected components: {connected_components(graph)}")


if __name__ == "__main__":
    main()
