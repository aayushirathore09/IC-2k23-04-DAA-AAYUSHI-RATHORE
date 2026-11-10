"""
Unit 5 | Experiment 1: Binary Tree creation and traversals.

Implements
  * Binary tree creation from a level-order list (None = missing child)
  * Preorder, Inorder, Postorder traversal  -> recursive AND iterative
  * Level-order traversal (bonus, uses a queue)
  * Verification: recursive result must equal iterative result

Usage:
    python tree_traversals.py            # interactive input
    python tree_traversals.py --demo     # built-in sample tree
"""
import sys
from collections import deque

NULL_TOKENS = {"null", "none", "-", "#", "n"}


class Node:
    """A single node of a binary tree."""
    __slots__ = ("data", "left", "right")

    def __init__(self, data):
        self.data = data
        self.left = None
        self.right = None


# --------------------------------------------------------------------------
# Creation
# --------------------------------------------------------------------------
def build_tree(values):
    """Build a binary tree from a level-order list. None means 'no child'.

    Time: O(n)   Space: O(n) for the queue + nodes.
    """
    if not isinstance(values, (list, tuple)):
        raise TypeError("values must be a list or tuple")
    if not values or values[0] is None:
        return None                      # empty tree
    root = Node(values[0])
    queue = deque([root])
    i = 1
    while queue and i < len(values):
        current = queue.popleft()
        if i < len(values) and values[i] is not None:
            current.left = Node(values[i])
            queue.append(current.left)
        i += 1
        if i < len(values) and values[i] is not None:
            current.right = Node(values[i])
            queue.append(current.right)
        i += 1
    return root


def parse_values(text):
    """Parse '1 2 3 null 4' (spaces or commas) into [1, 2, 3, None, 4]."""
    tokens = text.replace(",", " ").split()
    if not tokens:
        raise ValueError("No values entered")
    values = []
    for tok in tokens:
        if tok.lower() in NULL_TOKENS:
            values.append(None)
        else:
            try:
                values.append(int(tok))
            except ValueError:
                values.append(tok)       # allow string labels too
    if values[0] is None:
        raise ValueError("The root value cannot be null")
    return values


# --------------------------------------------------------------------------
# Recursive traversals  (O(n) time, O(h) stack space)
# --------------------------------------------------------------------------
def preorder_recursive(node, out=None):
    out = [] if out is None else out
    if node:
        out.append(node.data)                    # Root
        preorder_recursive(node.left, out)       # Left
        preorder_recursive(node.right, out)      # Right
    return out


def inorder_recursive(node, out=None):
    out = [] if out is None else out
    if node:
        inorder_recursive(node.left, out)        # Left
        out.append(node.data)                    # Root
        inorder_recursive(node.right, out)       # Right
    return out


def postorder_recursive(node, out=None):
    out = [] if out is None else out
    if node:
        postorder_recursive(node.left, out)      # Left
        postorder_recursive(node.right, out)     # Right
        out.append(node.data)                    # Root
    return out


# --------------------------------------------------------------------------
# Iterative traversals  (explicit stack; O(n) time, O(h) space)
# --------------------------------------------------------------------------
def preorder_iterative(root):
    result, stack = [], ([root] if root else [])
    while stack:
        node = stack.pop()
        result.append(node.data)
        if node.right:                           # push right first so left pops first
            stack.append(node.right)
        if node.left:
            stack.append(node.left)
    return result


def inorder_iterative(root):
    result, stack, current = [], [], root
    while current or stack:
        while current:                           # go as far left as possible
            stack.append(current)
            current = current.left
        current = stack.pop()
        result.append(current.data)
        current = current.right
    return result


def postorder_iterative(root):
    result, stack = [], []
    current, last_visited = root, None
    while current or stack:
        if current:
            stack.append(current)
            current = current.left
        else:
            peek = stack[-1]
            if peek.right and last_visited is not peek.right:
                current = peek.right             # right subtree not done yet
            else:
                result.append(peek.data)
                last_visited = stack.pop()
    return result


def level_order(root):
    """Breadth-first (level by level) traversal using a queue."""
    result, queue = [], deque([root] if root else [])
    while queue:
        node = queue.popleft()
        result.append(node.data)
        if node.left:
            queue.append(node.left)
        if node.right:
            queue.append(node.right)
    return result


# --------------------------------------------------------------------------
# Helpers
# --------------------------------------------------------------------------
def height(node):
    """Number of nodes on the longest root-to-leaf path (empty tree = 0)."""
    return 0 if node is None else 1 + max(height(node.left), height(node.right))


def count_nodes(node):
    return 0 if node is None else 1 + count_nodes(node.left) + count_nodes(node.right)


def print_tree(node, depth=0):
    """Print the tree rotated 90 degrees (right child on top)."""
    if node is None:
        return
    print_tree(node.right, depth + 1)
    print("    " * depth + str(node.data))
    print_tree(node.left, depth + 1)


def run_all(root):
    """Run every traversal, verify recursive == iterative, and print results."""
    pairs = [
        ("Preorder  (Root-Left-Right)", preorder_recursive(root), preorder_iterative(root)),
        ("Inorder   (Left-Root-Right)", inorder_recursive(root), inorder_iterative(root)),
        ("Postorder (Left-Right-Root)", postorder_recursive(root), postorder_iterative(root)),
    ]
    for name, rec, itr in pairs:
        print(f"{name}")
        print(f"   Recursive : {rec}")
        print(f"   Iterative : {itr}")
        print(f"   Verified  : {'OK - both match' if rec == itr else 'MISMATCH!'}")
        assert rec == itr, f"{name}: recursive and iterative results differ"
    print(f"Level-order (BFS)          : {level_order(root)}")


def main():
    if "--demo" in sys.argv:
        values = [1, 2, 3, 4, 5, None, 6, 7]
    else:
        try:
            text = input("Enter level-order values (use 'null' for a missing child,\n"
                         "e.g. 1 2 3 4 5 null 6) [Enter = demo]: ").strip()
        except EOFError:
            text = ""
        try:
            values = parse_values(text) if text else [1, 2, 3, 4, 5, None, 6, 7]
        except ValueError as err:
            print(f"Input error: {err}")
            return

    print(f"Level-order input : {values}")
    root = build_tree(values)
    print(f"Nodes = {count_nodes(root)},  Height = {height(root)}\n")
    print("Tree (rotated 90 deg, right child on top):")
    print_tree(root)
    print()
    run_all(root)


if __name__ == "__main__":
    main()
