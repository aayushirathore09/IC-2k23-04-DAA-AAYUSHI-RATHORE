# Unit 5 - Pseudocode

```text
PREORDER(n):  if n != NIL: visit(n); PREORDER(n.left); PREORDER(n.right)
INORDER(n):   if n != NIL: INORDER(n.left); visit(n); INORDER(n.right)
POSTORDER(n): if n != NIL: POSTORDER(n.left); POSTORDER(n.right); visit(n)

BFS(G, s)
  visited[s] <- true ; dist[s] <- 0 ; ENQUEUE(Q, s)
  while Q not empty
      u <- DEQUEUE(Q)
      for each v in Adj[u]
          if not visited[v]: visited[v] <- true ; dist[v] <- dist[u]+1 ; parent[v] <- u ; ENQUEUE(Q, v)

DFS-VISIT(u)
  visited[u] <- true ; disc[u] <- ++time
  for each v in Adj[u]: if not visited[v]: parent[v] <- u ; DFS-VISIT(v)
  fin[u] <- ++time
```
