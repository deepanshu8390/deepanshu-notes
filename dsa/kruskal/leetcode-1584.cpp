#include <bits/stdc++.h>
using namespace std;

class DSU {
    vector<int> parent;
    vector<int> size;

public:
    DSU(int n) {
        parent.resize(n);
        size.resize(n, 1);

        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
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

        // Every pair of points is a possible edge.
        // edge = {u, v, weight}
        vector<vector<int>> edges;

        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                int weight =
                    abs(points[i][0] - points[j][0]) +
                    abs(points[i][1] - points[j][1]);

                edges.push_back({i, j, weight});
            }
        }

        sort(edges.begin(), edges.end(), [](const vector<int>& a,
                                             const vector<int>& b) {
            return a[2] < b[2];
        });

        DSU dsu(n);

        int totalCost = 0;
        int edgesUsed = 0;

        for (auto& edge : edges) {
            int u = edge[0];
            int v = edge[1];
            int weight = edge[2];

            if (dsu.unite(u, v)) {
                totalCost += weight;
                edgesUsed++;

                // MST for N nodes always has N - 1 edges.
                if (edgesUsed == n - 1)
                    break;
            }
        }

        return totalCost;
    }
};
