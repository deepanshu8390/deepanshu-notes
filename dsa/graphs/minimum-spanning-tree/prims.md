# Prim's Algorithm

## Core Intuition

Prim's Algorithm builds one Minimum Spanning Tree by growing a connected component.

```text
Start with any node
        ↓
Look at edges leaving the current tree
        ↓
Choose the cheapest available edge
        ↓
Add the new node
        ↓
Repeat until all nodes are connected
```

### Prim vs Kruskal

```text
Kruskal → edge-centric → global cheapest edge → DSU
Prim    → neighbour/tree-centric → cheapest outgoing edge → min-heap
```

Both produce an MST, so the minimum total cost is the same (assuming the graph is connected).

## Key Observation

If the graph is naturally given as an adjacency list, Prim is a natural choice because we can inspect the neighbours of the current connected tree and keep candidate edges in a min-heap.

A normal BFS/DFS queue/stack is not enough: Prim must prioritize by **edge weight**, not by level or depth.

## Dry-run intuition

For a current tree `{A, B, D}`, do not look only at the latest node. Consider all edges going from the current tree to unvisited nodes. Pick the cheapest one.

```text
Current tree: {A,B,D}

A → C = 5
D → C = 3

Choose D-C = 3
```

## Standard C++

```cpp
int prim(int n, vector<vector<pair<int,int>>>& adj) {
    vector<bool> visited(n, false);

    priority_queue<
        pair<int,int>,
        vector<pair<int,int>>,
        greater<pair<int,int>>
    > pq;

    pq.push({0, 0});

    int cost = 0;

    while (!pq.empty()) {
        auto [weight, node] = pq.top();
        pq.pop();

        if (visited[node])
            continue;

        visited[node] = true;
        cost += weight;

        for (auto [next, edgeWeight] : adj[node]) {
            if (!visited[next]) {
                pq.push({edgeWeight, next});
            }
        }
    }

    return cost;
}
```

## Standard Java

```java
int prim(int n, List<List<int[]>> adj) {
    boolean[] visited = new boolean[n];

    PriorityQueue<int[]> pq = new PriorityQueue<>(
        (a, b) -> Integer.compare(a[0], b[0])
    );

    // {weight, node}
    pq.offer(new int[]{0, 0});

    int cost = 0;

    while (!pq.isEmpty()) {
        int[] current = pq.poll();
        int weight = current[0];
        int node = current[1];

        if (visited[node])
            continue;

        visited[node] = true;
        cost += weight;

        for (int[] edge : adj.get(node)) {
            int next = edge[0];
            int edgeWeight = edge[1];

            if (!visited[next]) {
                pq.offer(new int[]{edgeWeight, next});
            }
        }
    }

    return cost;
}
```

## Complexity

With adjacency list + min-heap: **O(E log E)** time and **O(E + V)** space in the standard implementation.
