# LeetCode 1489 — Find Critical and Pseudo-Critical Edges in MST

## Problem Statement

You are given a connected weighted undirected graph with `n` vertices and edges:

```text
[u, v, weight]
```

Find two groups of edges:

- **Critical edge:** removing it makes the MST total weight larger (or makes an MST impossible).
- **Pseudo-critical edge:** it can belong to at least one MST, but it is not critical.

Return the indices of the critical and pseudo-critical edges.

## Kruskal Connection

This problem uses the same Kruskal engine repeatedly.

For each edge:

```text
Force this edge first
    ↓
run Kruskal
    ↓
Can we still build an MST of the original minimum cost?
```

To test criticality:

```text
Skip this edge
    ↓
run Kruskal
    ↓
If MST cost becomes larger → critical
```

The problem is mainly about **reusing Kruskal with different constraints**.

## C++

```cpp
#include <bits/stdc++.h>
using namespace std;

class DSU {
    vector<int> parent, size;

public:
    DSU(int n) {
        parent.resize(n);
        size.resize(n, 1);
        for (int i = 0; i < n; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    bool unite(int u, int v) {
        int a = find(u);
        int b = find(v);

        if (a == b)
            return false;

        if (size[a] < size[b])
            swap(a, b);

        parent[b] = a;
        size[a] += size[b];
        return true;
    }
};

class Solution {
    int kruskal(int n, vector<vector<int>>& edges,
                int skip, int force) {
        DSU dsu(n);
        int cost = 0;
        int used = 0;

        if (force != -1) {
            auto& e = edges[force];
            if (dsu.unite(e[0], e[1])) {
                cost += e[2];
                used++;
            }
        }

        for (int i = 0; i < edges.size(); i++) {
            if (i == skip || i == force)
                continue;

            auto& e = edges[i];
            if (dsu.unite(e[0], e[1])) {
                cost += e[2];
                used++;

                if (used == n - 1)
                    break;
            }
        }

        return used == n - 1 ? cost : INT_MAX;
    }

public:
    vector<vector<int>> findCriticalAndPseudoCriticalEdges(
        int n, vector<vector<int>>& edges) {

        // Store original index.
        for (int i = 0; i < edges.size(); i++)
            edges[i].push_back(i);

        sort(edges.begin(), edges.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return a[2] < b[2];
             });

        int baseCost = kruskal(n, edges, -1, -1);
        vector<int> critical, pseudo;

        for (int i = 0; i < edges.size(); i++) {
            int without = kruskal(n, edges, i, -1);

            if (without > baseCost) {
                critical.push_back(edges[i][3]);
                continue;
            }

            int with = kruskal(n, edges, -1, i);
            if (with == baseCost)
                pseudo.push_back(edges[i][3]);
        }

        return {critical, pseudo};
    }
};
```

## Java

```java
import java.util.*;

class Solution {

    class DSU {
        int[] parent, size;

        DSU(int n) {
            parent = new int[n];
            size = new int[n];
            for (int i = 0; i < n; i++) {
                parent[i] = i;
                size[i] = 1;
            }
        }

        int find(int x) {
            if (parent[x] == x)
                return x;
            return parent[x] = find(parent[x]);
        }

        boolean union(int u, int v) {
            int a = find(u);
            int b = find(v);

            if (a == b)
                return false;

            if (size[a] < size[b]) {
                int temp = a;
                a = b;
                b = temp;
            }

            parent[b] = a;
            size[a] += size[b];
            return true;
        }
    }

    int kruskal(int n, int[][] edges, int skip, int force) {
        DSU dsu = new DSU(n);
        int cost = 0;
        int used = 0;

        if (force != -1) {
            int[] e = edges[force];
            if (dsu.union(e[0], e[1])) {
                cost += e[2];
                used++;
            }
        }

        for (int i = 0; i < edges.length; i++) {
            if (i == skip || i == force)
                continue;

            int[] e = edges[i];
            if (dsu.union(e[0], e[1])) {
                cost += e[2];
                used++;

                if (used == n - 1)
                    break;
            }
        }

        return used == n - 1 ? cost : Integer.MAX_VALUE;
    }

    public List<List<Integer>> findCriticalAndPseudoCriticalEdges(
            int n, int[][] input) {

        int m = input.length;
        int[][] edges = new int[m][4];

        for (int i = 0; i < m; i++) {
            edges[i][0] = input[i][0];
            edges[i][1] = input[i][1];
            edges[i][2] = input[i][2];
            edges[i][3] = i;
        }

        Arrays.sort(edges, (a, b) -> Integer.compare(a[2], b[2]));

        int baseCost = kruskal(n, edges, -1, -1);
        List<Integer> critical = new ArrayList<>();
        List<Integer> pseudo = new ArrayList<>();

        for (int i = 0; i < m; i++) {
            int without = kruskal(n, edges, i, -1);

            if (without > baseCost) {
                critical.add(edges[i][3]);
                continue;
            }

            int with = kruskal(n, edges, -1, i);
            if (with == baseCost)
                pseudo.add(edges[i][3]);
        }

        return Arrays.asList(critical, pseudo);
    }
}
```

## Complexity

There are `E` edges, and Kruskal is run a linear number of times.

```text
Sorting: O(E log E)
Repeated Kruskal: O(E² α(N)) amortized
Space: O(E + N)
```
