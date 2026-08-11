# Shortest Path

## 3 ways hote hain

1. **Unweighted → BFS**
2. **0/1 → 0-1 BFS + Deque**
3. **Positive weights → Dijkstra + Min Heap**

---

## 1. Unweighted Graph → BFS

### Intuition

Har edge ka cost `1` hai.

```text
Source
  ↓
Distance 1
  ↓
Distance 2
  ↓
Distance 3
```

BFS level-by-level chalta hai, so **first visit = shortest distance**.

### C++

```cpp
vector<int> shortestPath(int n, vector<vector<int>>& adj, int source) {

    vector<int> dist(n, -1);
    queue<int> q;

    dist[source] = 0;
    q.push(source);

    while (!q.empty()) {

        int node = q.front();
        q.pop();

        for (int neighbour : adj[node]) {

            if (dist[neighbour] == -1) {

                dist[neighbour] = dist[node] + 1;
                q.push(neighbour);
            }
        }
    }

    return dist;
}
```

### Java

```java
int[] shortestPath(int n, ArrayList<ArrayList<Integer>> adj, int source) {

    int[] dist = new int[n];
    Arrays.fill(dist, -1);

    Queue<Integer> q = new LinkedList<>();

    dist[source] = 0;
    q.add(source);

    while (!q.isEmpty()) {

        int node = q.poll();

        for (int neighbour : adj.get(node)) {

            if (dist[neighbour] == -1) {

                dist[neighbour] = dist[node] + 1;
                q.add(neighbour);
            }
        }
    }

    return dist;
}
```

**TC:** `O(V + E)`  
**SC:** `O(V)`

---

## 2. 0/1 Graph → 0-1 BFS + Deque

### Intuition

Edge weight sirf `0` ya `1` hota hai.

```text
0 = VIP → front
1 = normal → back
```

Isliye normal queue ki jagah `deque`.

```text
weight 0 → push_front()
weight 1 → push_back()
```

### C++

```cpp
vector<int> shortestPath01(
    int n,
    vector<vector<pair<int, int>>>& adj,
    int source
) {
    vector<int> dist(n, INT_MAX);

    deque<int> dq;

    dist[source] = 0;
    dq.push_front(source);

    while (!dq.empty()) {

        int node = dq.front();
        dq.pop_front();

        for (auto [neighbour, weight] : adj[node]) {

            int newDist = dist[node] + weight;

            if (newDist < dist[neighbour]) {

                dist[neighbour] = newDist;

                if (weight == 0)
                    dq.push_front(neighbour);
                else
                    dq.push_back(neighbour);
            }
        }
    }

    return dist;
}
```

### Java

```java
int[] shortestPath01(
    int n,
    ArrayList<ArrayList<int[]>> adj,
    int source
) {
    int[] dist = new int[n];
    Arrays.fill(dist, Integer.MAX_VALUE);

    Deque<Integer> dq = new ArrayDeque<>();

    dist[source] = 0;
    dq.addFirst(source);

    while (!dq.isEmpty()) {

        int node = dq.removeFirst();

        for (int[] edge : adj.get(node)) {

            int neighbour = edge[0];
            int weight = edge[1];

            int newDist = dist[node] + weight;

            if (newDist < dist[neighbour]) {

                dist[neighbour] = newDist;

                if (weight == 0)
                    dq.addFirst(neighbour);
                else
                    dq.addLast(neighbour);
            }
        }
    }

    return dist;
}
```

**TC:** `O(V + E)`  
**SC:** `O(V)`

---

## 3. Positive Weights → Dijkstra

### Intuition

Weights positive hain: `2, 5, 10, 20...`

Har baar:

> **Currently jis node ki distance sabse minimum hai, usko process karo.**

Isliye **Min Heap / Priority Queue**.

Neighbour ke liye:

```text
newDist = currentDist + edgeWeight
```

Agar better distance mila:

```text
dist[neighbour] = newDist
```

### C++

```cpp
vector<int> dijkstra(
    int n,
    vector<vector<pair<int, int>>>& adj,
    int source
) {
    vector<int> dist(n, INT_MAX);

    priority_queue<
        pair<int, int>,
        vector<pair<int, int>>,
        greater<pair<int, int>>
    > pq;

    dist[source] = 0;
    pq.push({0, source});

    while (!pq.empty()) {

        auto [currentDist, node] = pq.top();
        pq.pop();

        if (currentDist > dist[node])
            continue;

        for (auto [neighbour, weight] : adj[node]) {

            int newDist = currentDist + weight;

            if (newDist < dist[neighbour]) {

                dist[neighbour] = newDist;

                pq.push({newDist, neighbour});
            }
        }
    }

    return dist;
}
```

### Java

```java
int[] dijkstra(
    int n,
    ArrayList<ArrayList<int[]>> adj,
    int source
) {
    int[] dist = new int[n];
    Arrays.fill(dist, Integer.MAX_VALUE);

    PriorityQueue<int[]> pq =
        new PriorityQueue<>((a, b) -> a[0] - b[0]);

    dist[source] = 0;
    pq.offer(new int[]{0, source});

    while (!pq.isEmpty()) {

        int[] current = pq.poll();

        int currentDist = current[0];
        int node = current[1];

        if (currentDist > dist[node])
            continue;

        for (int[] edge : adj.get(node)) {

            int neighbour = edge[0];
            int weight = edge[1];

            int newDist = currentDist + weight;

            if (newDist < dist[neighbour]) {

                dist[neighbour] = newDist;

                pq.offer(new int[]{newDist, neighbour});
            }
        }
    }

    return dist;
}
```

**TC:** `O((V + E) log V)`  
**SC:** `O(V + E)` including adjacency list.

---

## Recognition Cheat Sheet

```text
Shortest Path
│
├── Every edge = 1
│      ↓
│     BFS
│
├── Edge = 0 or 1
│      ↓
│   0-1 BFS
│      ↓
│    Deque
│    0 → front
│    1 → back
│
└── Positive arbitrary weights
       ↓
    Dijkstra
       ↓
    Min Heap
```

**Important:** Dijkstra ke liye edge weights **non-negative** hone chahiye. Negative weights aaye toh Dijkstra nahi.