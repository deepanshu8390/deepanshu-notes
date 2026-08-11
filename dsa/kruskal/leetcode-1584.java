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

        // Every pair of points is a possible edge.
        // edge = {u, v, weight}
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
            int u = edge[0];
            int v = edge[1];
            int weight = edge[2];

            if (dsu.union(u, v)) {
                totalCost += weight;
                edgesUsed++;

                // MST for N nodes always has N - 1 edges.
                if (edgesUsed == n - 1)
                    break;
            }
        }

        return totalCost;
    }
}
