"""
Unit 5 | Application: Graph Exploration / Network Traversal System.

Models a computer network (devices = vertices, links = edges) and uses the
Unit-5 algorithms to answer practical questions:

  1. Reachability   - which devices can a source reach, and how many hops away? (BFS)
  2. Best route     - fewest-hop route between two devices                       (BFS)
  3. Exploration    - depth-first scan order of the network                      (DFS)
  4. Segments       - isolated / disconnected parts of the network               (components)
  5. Hop-limited    - devices within k hops of a source (e.g. broadcast range)   (BFS)

Usage:
    python network_explorer.py                      # menu, loads network.txt
    python network_explorer.py --file my_net.txt    # menu, your own network file
    python network_explorer.py --demo               # run every feature once
File format: one link per line 'DeviceA DeviceB'; '#' starts a comment.
"""
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
for folder in ("02-Graph-Representation", "03-BFS", "04-DFS"):
    sys.path.insert(0, os.path.join(HERE, "..", folder))
from graph_representation import AdjacencyList                      # noqa: E402
from bfs import bfs, shortest_path, levels, connected_components    # noqa: E402
from dfs import dfs_iterative                                       # noqa: E402


class Network:
    """Maps device names to vertex numbers and wraps an AdjacencyList."""

    def __init__(self, links):
        self.names = []
        self.index = {}
        for a, b in links:
            for name in (a, b):
                if name not in self.index:
                    self.index[name] = len(self.names)
                    self.names.append(name)
        if not self.names:
            raise ValueError("The network has no devices")
        self.graph = AdjacencyList.from_edges(
            len(self.names), [(self.index[a], self.index[b]) for a, b in links])

    @classmethod
    def from_file(cls, path):
        links = []
        with open(path, encoding="utf-8") as handle:
            for line_no, line in enumerate(handle, 1):
                line = line.split("#", 1)[0].strip()
                if not line:
                    continue
                parts = line.split()
                if len(parts) != 2:
                    raise ValueError(f"{path}, line {line_no}: expected 'DeviceA DeviceB'")
                links.append((parts[0], parts[1]))
        return cls(links)

    def vertex(self, name):
        if name not in self.index:
            raise KeyError(f"Unknown device '{name}'. Known: {', '.join(self.names)}")
        return self.index[name]

    def label(self, vertices):
        return [self.names[v] for v in vertices]


# --------------------------------------------------------------------------
# Features
# --------------------------------------------------------------------------
def reachability(net, source):
    result = bfs(net.graph, net.vertex(source))
    print(f"Devices reachable from {source} (by hop count):")
    for hops, group in sorted(levels(result).items()):
        print(f"  {hops} hop(s): {', '.join(net.label(group))}")
    lost = [v for v in range(net.graph.n) if result["dist"][v] == -1]
    print(f"  Not reachable: {', '.join(net.label(lost)) if lost else 'none'}")


def best_route(net, source, target):
    result = bfs(net.graph, net.vertex(source))
    path = shortest_path(result, net.vertex(source), net.vertex(target))
    if path is None:
        print(f"No route between {source} and {target}.")
    else:
        print(f"Fewest-hop route ({len(path) - 1} hops): {' -> '.join(net.label(path))}")


def exploration(net, source):
    result = dfs_iterative(net.graph, net.vertex(source))
    print(f"DFS scan order from {source}: {' -> '.join(net.label(result['order']))}")


def segments(net):
    parts = connected_components(net.graph)
    print(f"The network has {len(parts)} connected segment(s):")
    for i, part in enumerate(parts, 1):
        print(f"  Segment {i} ({len(part)} devices): {', '.join(net.label(part))}")


def within_hops(net, source, limit):
    result = bfs(net.graph, net.vertex(source))
    near = [v for v in result["order"] if 0 < result["dist"][v] <= limit]
    print(f"Devices within {limit} hop(s) of {source}: {', '.join(net.label(near)) if near else 'none'}")


# --------------------------------------------------------------------------
def ask(prompt, net=None):
    """Read one line; for device names re-ask until the device exists."""
    while True:
        try:
            value = input(prompt).strip()
        except EOFError:
            raise SystemExit("\nInput ended.")
        if net is None or value in net.index:
            return value
        print(f"  Unknown device. Choose from: {', '.join(net.names)}")


def menu(net):
    options = {
        "1": "Reachability from a device (BFS)",
        "2": "Fewest-hop route between two devices (BFS)",
        "3": "Depth-first scan of the network (DFS)",
        "4": "Find disconnected segments",
        "5": "Devices within k hops",
        "0": "Exit",
    }
    while True:
        print("\n=== Network Traversal System ===")
        for key, text in options.items():
            print(f"  {key}. {text}")
        choice = input("Choice: ").strip()
        print()
        if choice == "1":
            reachability(net, ask("Source device: ", net))
        elif choice == "2":
            best_route(net, ask("From: ", net), ask("To: ", net))
        elif choice == "3":
            exploration(net, ask("Start device: ", net))
        elif choice == "4":
            segments(net)
        elif choice == "5":
            source = ask("Source device: ", net)
            try:
                within_hops(net, source, int(input("k (hops): ")))
            except ValueError:
                print("k must be a whole number.")
        elif choice == "0":
            break
        else:
            print("Invalid choice.")


def demo(net):
    print(f"Loaded network: {net.graph.n} devices, {net.graph.edge_count} links\n")
    reachability(net, "Gateway")
    print()
    best_route(net, "Gateway", "PC4")
    print()
    best_route(net, "Gateway", "PrinterA")
    print()
    exploration(net, "Gateway")
    print()
    segments(net)
    print()
    within_hops(net, "CoreSwitch", 1)


def main():
    path = os.path.join(HERE, "network.txt")
    if "--file" in sys.argv:
        try:
            path = sys.argv[sys.argv.index("--file") + 1]
        except IndexError:
            raise SystemExit("--file needs a path")
    try:
        net = Network.from_file(path)
    except (OSError, ValueError) as err:
        raise SystemExit(f"Could not load network: {err}")
    if "--demo" in sys.argv:
        demo(net)
    else:
        print(f"Loaded network: {net.graph.n} devices, {net.graph.edge_count} links")
        menu(net)


if __name__ == "__main__":
    main()
