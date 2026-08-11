# LeetCode 1584 — Min Cost to Connect All Points

## Problem Statement

You are given points on a 2D plane. Treat each point as a node. Any pair of points can be connected, and the cost of connecting two points is their Manhattan distance:

```text
|x1 - x2| + |y1 - y2|
```

Connect all points with the minimum total cost.

This is an MST problem. For Kruskal, generate every possible pair as an edge, store `{u, v, weight}`, sort by weight, then use DSU to accept only edges that connect different components.

## C++

```cpp
#include <bits/stdc++.h>
using namespace std;

class DSU {
    vector<int> parent;
    vector<int> size;

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
    int minCostConnectPoints(vector<vector<int>>& points) {
        int n = points.size();
        vector<vector<int>> edges;

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int weight =
                    abs(points[i][0] - points[j][0]) +
                    abs(points[i][1] - points[j][1]);

                edges.push_back({i, j, weight});
            }
        }

        sort(edges.begin(), edges.end(),
             [](const vector<int>& a, const vector<int>& b) {
                 return a[2] < b[2];
             });

        DSU dsu(n);
        int totalCost = 0;
        int edgesUsed = 0;

        for (auto& edge : edges) {
            if (dsu.unite(edge[0], edge[1])) {
                totalCost += edge[2];
                edgesUsed++;

                if (edgesUsed == n - 1)
                    break;
            }
        }

        return totalCost;
    }
};
```

## Java

```java
import java.util.*;

class Solution {

    class DSU {
        int[] parent;
        int[] size;

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

    public int minCostConnectPoints(int[][] points) {
        int n = points.length;
        List<int[]> edges = new ArrayList<>();

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int weight =
                    Math.abs(points[i][0] - points[j][0]) +
                    Math.abs(points[i][1] - points[j][1]);

                edges.add(new int[]{i, j, weight});
            }
        }

        edges.sort((a, b) -> Integer.compare(a[2], b[2]));

        DSU dsu = new DSU(n);
        int totalCost = 0;
        int edgesUsed = 0;

        for (int[] edge : edges) {
            if (dsu.union(edge[0], edge[1])) {
                totalCost += edge[2];
                edgesUsed++;

                if (edgesUsed == n - 1)
                    break;
            }
        }

        return totalCost;
    }
}
```

## Complexity

There are `E = N(N-1)/2 = O(N²)` possible edges.

```text
Generate edges: O(N²)
Sort:           O(N² log N)
DSU:            O(N² α(N)) amortized
Space:          O(N²)
```
