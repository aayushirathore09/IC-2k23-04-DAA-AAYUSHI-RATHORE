# Unit 5 - Complexity Summary

| Algorithm | Time | Extra space |
|---|---|---|
| Tree traversals (all orders, recursive or iterative) | Theta(n) | O(h) |
| BFS / DFS, adjacency list | Theta(V + E) | O(V) |
| BFS / DFS, adjacency matrix | Theta(V^2) | O(V) |
| Graph storage, adjacency list | - | O(V + E) |
| Graph storage, adjacency matrix | - | O(V^2) |

Empirical check (see `Unit05-Traversal-and-Search/05-Performance-Analysis`): linear fit of BFS time against theoretical operation count gives R^2 = 0.9702 (list) and 0.9851 (matrix).
