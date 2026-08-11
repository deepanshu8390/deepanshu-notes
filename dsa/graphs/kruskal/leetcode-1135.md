# LeetCode 1135 — Connecting Cities With Minimum Cost

## Problem Statement

There are `n` cities. Some pairs of cities can be connected with a given cost.

Choose connections so that **all cities become connected** with the **minimum total cost**.

Each connection is given as:

```text
[u, v, cost]
```

Return the minimum cost. If it is impossible to connect all cities, return `-1`.

## Kruskal Feel

This is a direct MST problem:

```text
minimum total cost
+
all cities connected
        ↓
       MST
        ↓
    Kruskal
```

Process edges from cheapest to most expensive. If the endpoints are already in the same component, reject the edge because it creates a cycle. Otherwise accept it and merge the components using DSU.

## C++

```cpp
#include <bits/stdc++.h>
using namespace std;

class DSU {
    vector<int> parent, size;

public:
    DSU(int n) {
        parent.resize(n + 1);
        size.resize(n + 1, 1);
        for (int i = 1; i <= n; i++)
            parent[i] = i;
    }

    int find(int x) {
        if (parent[x] == x)
            return x;
        return parent[x] = find(parent[x]);
    }

    bool unite(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);

        if (rootU == rootV)
            return false;

        if (size[rootU] < size[rootV])
            swap(rootU, rootV);

        parent[rootV] = rootU;
        size[rootU] += size[rootV];
        return true;
    }
};

class Solution {
public:
    int minimumCost(int n, vector<vector<int>>& connections) {
        sort(connections.begin(), connections.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return a[2] < b[2];
             });

        DSU dsu(n);
        int totalCost = 0;
        int edgesUsed = 0;

        for (auto& edge : connections) {
            if (dsu.unite(edge[0], edge[1])) {
                totalCost += edge[2];
                edgesUsed++;
            }
        }

        return edgesUsed == n - 1 ? totalCost : -1;
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
            parent = new int[n + 1];
            size = new int[n + 1];

            for (int i = 1; i <= n; i++) {
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
            int rootU = find(u);
            int rootV = find(v);

            if (rootU == rootV)
                return false;

            if (size[rootU] < size[rootV]) {
                int temp = rootU;
                rootU = rootV;
                rootV = temp;
            }

            parent[rootV] = rootU;
            size[rootU] += size[rootV];
            return true;
        }
    }

    public int minimumCost(int n, int[][] connections) {
        Arrays.sort(connections, (a, b) -> Integer.compare(a[2], b[2]));

        DSU dsu = new DSU(n);
        int totalCost = 0;
        int edgesUsed = 0;

        for (int[] edge : connections) {
            if (dsu.union(edge[0], edge[1])) {
                totalCost += edge[2];
                edgesUsed++;
            }
        }

        return edgesUsed == n - 1 ? totalCost : -1;
    }
}
```

## Complexity

For `E` connections:

```text
Sorting: O(E log E)
DSU:     O(E α(N)) amortized
Space:   O(E + N)
```
