"""
Unit 5 | Experiment 2: Graph representation.

  * AdjacencyMatrix : V x V matrix,  space O(V^2)
  * AdjacencyList   : array of sorted neighbour lists, space O(V + E)

Both classes expose the SAME interface (add_edge, remove_edge, has_edge,
neighbors, edges, display) so BFS / DFS can run on either one.
Vertices are numbered 0 .. n-1.  Graphs are unweighted.

Usage:
    python graph_representation.py          # interactive
    python graph_representation.py --demo   # built-in example
"""
import sys
from bisect import bisect_left, insort

# Sample graph shared by all Unit-5 programs: two components {0..7} and {8,9}
DEMO_N = 10
DEMO_EDGES = [(0, 1), (0, 2), (1, 3), (1, 4), (2, 5), (2, 6),
              (3, 7), (4, 7), (5, 6), (8, 9)]


class Graph:
    """Common behaviour shared by both representations."""

    def __init__(self, n, directed=False):
        if not isinstance(n, int) or isinstance(n, bool) or n <= 0:
            raise ValueError("Number of vertices must be a positive integer")
        self.n = n
        self.directed = directed
        self.edge_count = 0

    def validate_vertex(self, u):
        if not isinstance(u, int) or isinstance(u, bool) or not 0 <= u < self.n:
            raise ValueError(f"Vertex {u!r} is invalid (valid range: 0..{self.n - 1})")

    @classmethod
    def from_edges(cls, n, edges, directed=False):
        graph = cls(n, directed)
        for u, v in edges:
            graph.add_edge(u, v)
        return graph

    def degree(self, u):
        self.validate_vertex(u)
        return len(self.neighbors(u))

    def edges(self):
        """Return the edge list (undirected edges are reported once, u <= v)."""
        return [(u, v) for u in range(self.n) for v in self.neighbors(u)
                if self.directed or u <= v]


class AdjacencyMatrix(Graph):
    """matrix[u][v] = 1 if edge u->v exists else 0.   Space: O(V^2)."""

    def __init__(self, n, directed=False):
        super().__init__(n, directed)
        self.matrix = [[0] * n for _ in range(n)]

    def add_edge(self, u, v):                       # O(1)
        self.validate_vertex(u)
        self.validate_vertex(v)
        if self.matrix[u][v]:
            return False                            # duplicate edge ignored
        self.matrix[u][v] = 1
        if not self.directed:
            self.matrix[v][u] = 1
        self.edge_count += 1
        return True

    def remove_edge(self, u, v):                    # O(1)
        self.validate_vertex(u)
        self.validate_vertex(v)
        if not self.matrix[u][v]:
            return False
        self.matrix[u][v] = 0
        if not self.directed:
            self.matrix[v][u] = 0
        self.edge_count -= 1
        return True

    def has_edge(self, u, v):                       # O(1)
        self.validate_vertex(u)
        self.validate_vertex(v)
        return bool(self.matrix[u][v])

    def neighbors(self, u):                         # O(V): must scan the whole row
        return [v for v, flag in enumerate(self.matrix[u]) if flag]

    def stored_entries(self):
        return self.n * self.n

    def display(self):
        print("Adjacency Matrix:")
        print("     " + " ".join(f"{j:2d}" for j in range(self.n)))
        for i, row in enumerate(self.matrix):
            print(f"{i:3d}: " + " ".join(f"{x:2d}" for x in row))


class AdjacencyList(Graph):
    """adj[u] = sorted list of neighbours of u.   Space: O(V + E)."""

    def __init__(self, n, directed=False):
        super().__init__(n, directed)
        self.adj = [[] for _ in range(n)]

    def _contains(self, u, v):
        row = self.adj[u]
        i = bisect_left(row, v)
        return i < len(row) and row[i] == v

    def add_edge(self, u, v):                       # O(deg) because lists stay sorted
        self.validate_vertex(u)
        self.validate_vertex(v)
        if self._contains(u, v):
            return False
        insort(self.adj[u], v)
        if not self.directed and u != v:
            insort(self.adj[v], u)
        self.edge_count += 1
        return True

    def remove_edge(self, u, v):                    # O(deg)
        self.validate_vertex(u)
        self.validate_vertex(v)
        if not self._contains(u, v):
            return False
        self.adj[u].remove(v)
        if not self.directed and u != v:
            self.adj[v].remove(u)
        self.edge_count -= 1
        return True

    def has_edge(self, u, v):                       # O(log deg)
        self.validate_vertex(u)
        self.validate_vertex(v)
        return self._contains(u, v)

    def neighbors(self, u):                         # O(1) - returns the stored list
        return self.adj[u]                          # (treat as read-only)

    def stored_entries(self):
        return self.n + sum(len(r) for r in self.adj)

    def display(self):
        print("Adjacency List:")
        for u in range(self.n):
            print(f"{u:3d} -> {self.adj[u]}")


# --------------------------------------------------------------------------
# Input helpers (reused by BFS / DFS programs)
# --------------------------------------------------------------------------
def read_int(prompt, low, high=None):
    """Keep asking until a valid integer in [low, high] is entered."""
    while True:
        try:
            value = int(input(prompt))
        except ValueError:
            print("  Please enter a whole number.")
            continue
        except EOFError:
            raise SystemExit("\nInput ended.")
        if value < low or (high is not None and value > high):
            print(f"  Value must be between {low} and {high if high is not None else 'infinity'}.")
            continue
        return value


def read_graph_interactive():
    """Ask the user for n, directed/undirected and the edges; return (n, edges, directed)."""
    n = read_int("Number of vertices (V): ", 1)
    directed = input("Directed graph? (y/N): ").strip().lower() == "y"
    max_edges = n * n if directed else n * (n + 1) // 2
    m = read_int("Number of edges (E): ", 0, max_edges)
    edges = []
    print(f"Enter {m} edges as 'u v' (vertices 0..{n - 1}):")
    while len(edges) < m:
        parts = input(f"  edge {len(edges) + 1}: ").split()
        try:
            if len(parts) != 2:
                raise ValueError("expected exactly two numbers")
            u, v = int(parts[0]), int(parts[1])
            if not (0 <= u < n and 0 <= v < n):
                raise ValueError(f"vertices must be in 0..{n - 1}")
        except ValueError as err:
            print(f"  Invalid edge: {err}")
            continue
        edges.append((u, v))
    return n, edges, directed


def get_graph_from_cli(representation="list"):
    """Used by BFS/DFS programs. '--demo' -> sample graph, else ask the user."""
    cls = AdjacencyMatrix if ("--matrix" in sys.argv or representation == "matrix") else AdjacencyList
    if "--demo" in sys.argv:
        return cls.from_edges(DEMO_N, DEMO_EDGES)
    n, edges, directed = read_graph_interactive()
    return cls.from_edges(n, edges, directed)


# --------------------------------------------------------------------------
def main():
    if "--demo" in sys.argv:
        n, edges, directed = DEMO_N, DEMO_EDGES, False
    else:
        n, edges, directed = read_graph_interactive()

    matrix = AdjacencyMatrix.from_edges(n, edges, directed)
    adjlist = AdjacencyList.from_edges(n, edges, directed)

    print(f"\nGraph: V = {n}, E = {matrix.edge_count}, {'directed' if directed else 'undirected'}\n")
    matrix.display()
    print()
    adjlist.display()

    print("\nDegrees (out-degree for directed graphs):")
    print("  " + ", ".join(f"deg({u})={adjlist.degree(u)}" for u in range(n)))

    # Verification: both representations must describe the same graph
    assert sorted(matrix.edges()) == sorted(adjlist.edges()), "Representations differ!"
    print("\nVerification: matrix and list contain the same edge set -> OK")

    print(f"\nSpace used  : matrix stores {matrix.stored_entries()} cells (V^2), "
          f"list stores {adjlist.stored_entries()} entries (V + {'E' if directed else '2E'})")

    if edges:
        u, v = edges[0]
        print(f"\nOperation demo on edge ({u}, {v}):")
        print(f"  has_edge({u},{v}) matrix={matrix.has_edge(u, v)}, list={adjlist.has_edge(u, v)}")
        matrix.remove_edge(u, v)
        adjlist.remove_edge(u, v)
        print(f"  after remove_edge -> matrix={matrix.has_edge(u, v)}, list={adjlist.has_edge(u, v)}, "
              f"E={adjlist.edge_count}")


if __name__ == "__main__":
    main()
