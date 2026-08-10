# Topological Sort

## 🧠 Basic Intuition — Problem se approach tak

Topological Sort ka simple requirement:

> Har directed edge `u → v` ke liye `u` ko `v` se pehle aana chahiye.

Example:

```text
A → B → C
```

Toh A ko B se pehle aur B ko C se pehle aana hi padega.

### Starting node ka thought

Sabse pehle kaun aa sakta hai?

> Jiski koi incoming dependency nahi hai.

Isliye:

```text
indegree = 0
```

aisa node source hai.

Agar multiple sources hain, toh unmein se koi bhi choose kar sakte hain.

---

# Kahn's Algorithm — BFS

## 🔥 Maine kaise derive kiya

Source mil gaya (`indegree = 0`). Ab usko process kar diya.

Suppose:

```text
A → B
```

A process ho gaya, matlab B ke liye A wali dependency complete ho gayi.

So:

```text
indegree[B]--
```

Agar B ka indegree `1 → 0` ho gaya, toh B ab ready hai.

Yahin se pattern naturally aata hai:

```text
indegree 0 node
      ↓
process
      ↓
neighbours ki indegree --
      ↓
jo 0 ho gaya → next ready node
```

### 💡 Important mental model

`indegree` ko sirf "incoming edges ki count" mat samjho.

Processing ke time ise aise feel karo:

> **Kitni dependencies abhi pending hain?**

---

## Queue kyun?

Ek time par multiple nodes ready ho sakte hain.

Example:

```text
A → C
B → C
```

Initially `A` aur `B` dono ready ho sakte hain.

Queue bas **ready nodes ki waiting line** hai.

---

# DFS — Same Topological Sort, Different Thinking

## ⚠️ DFS mein indegree kyun nahi?

Kahn ka question:

> **Kaun abhi ready hai?**

Isliye indegree maintain karte hain.

DFS ka question:

> **Current node se aage jo nodes hain, kya unko pehle complete kar sakte hain?**

DFS mein recursion stack already ye order naturally maintain karta hai.

```text
current
   ↓
neighbour
   ↓
next neighbour
   ↓
...
   ↓
return / backtrack
   ↓
current ko answer mein daalo
```

Isliye DFS ko `indegree--` karne ki zarurat nahi.

### 🔥 One-line difference

```text
Kahn → ready node dhoondo
DFS  → node ko complete karke answer mein daalo
```

---

## 🔄 DFS Dry Run 1 — Simple Chain

Graph:

```text
A → B → C
```

Call:

```text
DFS(A)
   ↓
 DFS(B)
   ↓
  DFS(C)
```

C ke neighbours nahi hain, so:

```text
C → answer
```

Return to B:

```text
B → answer
```

Return to A:

```text
A → answer
```

Temporary answer:

```text
C B A
```

Finally reverse:

```text
A B C
```

---

## 🔄 DFS Dry Run 2 — Branching

Graph:

```text
      A
     / \
    B   C
     \ /
      D
```

Edges:

```text
A → B
A → C
B → D
C → D
```

Suppose DFS starts from A and visits B first:

```text
A
↓
B
↓
D → answer
```

Return to B:

```text
B → answer
```

Back to A, now C:

```text
A
 \
  C
  ↓
  D already visited
```

C complete:

```text
C → answer
```

Finally A:

```text
A → answer
```

Temporary answer:

```text
D B C A
```

Reverse:

```text
A C B D
```

Valid topological order.

**Important:** `D` pehle answer mein aaya because DFS ne usko complete kiya; reverse ke baad D dependency ke baad aata hai.

---

## 🔄 DFS Dry Run 3 — Multiple Components

Graph:

```text
A → B

C → D
```

Pehle DFS(A):

```text
A → B
B → answer
A → answer
```

Answer:

```text
B A
```

Phir DFS(C):

```text
C → D
D → answer
C → answer
```

Answer:

```text
B A D C
```

Reverse:

```text
C D A B
```

Ye bhi valid hai because dono components independent hain.

---

## 🧠 Why DFS mein stack space?

DFS recursion use kar raha hai, so calls stack mein store hoti hain.

Example:

```text
A → B → C → D
```

Deepest moment par recursion stack:

```text
DFS(A)
DFS(B)
DFS(C)
DFS(D)
```

Maximum depth `V` tak ja sakti hai.

So **DFS recursion stack = O(V)** in worst case.

> Sirf recursion stack ki baat kar rahe ho toh stack space `O(V)` hai.
> Overall implementation mein `visited` bhi `O(V)` hota hai, aur answer `O(V)` output space leta hai.

Time:

```text
O(V + E)
```

because har node aur edge traversal mein consider hota hai.

---

# C++ — Kahn (BFS)

```cpp
vector<int> topoSort(int V, vector<int> adj[]) {
    vector<int> indegree(V, 0);

    for (int u = 0; u < V; u++) {
        for (auto v : adj[u]) {
            indegree[v]++;
        }
    }

    queue<int> q;

    for (int i = 0; i < V; i++) {
        if (indegree[i] == 0)
            q.push(i);
    }

    vector<int> ans;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        ans.push_back(u);

        for (auto v : adj[u]) {
            indegree[v]--;

            if (indegree[v] == 0)
                q.push(v);
        }
    }

    return ans;
}
```

---

# Java — Kahn (BFS)

```java
int[] topoSort(int V, ArrayList<ArrayList<Integer>> adj) {
    int[] indegree = new int[V];

    for (int u = 0; u < V; u++) {
        for (int v : adj.get(u)) {
            indegree[v]++;
        }
    }

    Queue<Integer> q = new LinkedList<>();

    for (int i = 0; i < V; i++) {
        if (indegree[i] == 0)
            q.add(i);
    }

    int[] ans = new int[V];
    int index = 0;

    while (!q.isEmpty()) {
        int u = q.poll();
        ans[index++] = u;

        for (int v : adj.get(u)) {
            indegree[v]--;

            if (indegree[v] == 0)
                q.add(v);
        }
    }

    return ans;
}
```

---

# C++ — DFS

```cpp
void dfs(int u, vector<int> adj[], vector<int>& vis, vector<int>& ans) {
    vis[u] = 1;

    for (auto v : adj[u]) {
        if (!vis[v])
            dfs(v, adj, vis, ans);
    }

    ans.push_back(u);
}

vector<int> topoSort(int V, vector<int> adj[]) {
    vector<int> vis(V, 0);
    vector<int> ans;

    for (int i = 0; i < V; i++) {
        if (!vis[i])
            dfs(i, adj, vis, ans);
    }

    reverse(ans.begin(), ans.end());
    return ans;
}
```

---

# Java — DFS

```java
void dfs(int u, ArrayList<ArrayList<Integer>> adj,
         boolean[] vis, ArrayList<Integer> ans) {
    vis[u] = true;

    for (int v : adj.get(u)) {
        if (!vis[v])
            dfs(v, adj, vis, ans);
    }

    ans.add(u);
}

ArrayList<Integer> topoSort(int V, ArrayList<ArrayList<Integer>> adj) {
    boolean[] vis = new boolean[V];
    ArrayList<Integer> ans = new ArrayList<>();

    for (int i = 0; i < V; i++) {
        if (!vis[i])
            dfs(i, adj, vis, ans);
    }

    Collections.reverse(ans);
    return ans;
}
```

---

## 📝 Things to Revise

- Topological Sort = every `u → v` means `u` before `v`.
- Source = `indegree == 0`.
- Kahn = BFS + indegree + queue of ready nodes.
- Processing a node means its outgoing neighbours lose one pending dependency.
- DFS = recursion stack; neighbours first, current node later.
- DFS does **not** need indegree.
- DFS answer is built in reverse order, so reverse it at the end.
- DFS recursion stack = `O(V)` worst case; overall auxiliary space also includes `visited`.
