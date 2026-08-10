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

## ⚠️ Important: DFS mein indegree nahi

Yahan Kahn aur DFS ko mix mat karna.

### Kahn / BFS

```text
source → indegree 0
      → process
      → indegree reduce
      → new source
```

### DFS

DFS mein hume dependency count maintain karne ki zarurat nahi.

Recursion stack khud ek natural order deta hai:

> **Pehle neighbours ko complete karo, phir current node ko answer mein daalo.**

Example 1:

```text
A → B → C
```

DFS flow:

```text
A
 ↓
B
 ↓
C → answer
  B → answer
    A → answer
```

Answer initially:

```text
C B A
```

Reverse karne par:

```text
A B C
```

Example 2:

```text
A → C
B → C
```

DFS `C` ko dono parents se pehle complete karega. Agar traversal A se start hua:

```text
A → C → answer
B → answer
```

Temporary answer:

```text
C A B
```

Reverse:

```text
B A C
```

Ye valid topological order hai.

### 🔥 Why no indegree in DFS?

Kahn ka question hai:

> **Kaun abhi ready hai?**

Isliye indegree maintain karte hain.

DFS ka question hai:

> **Is node ke saare dependent/next nodes complete hue ya nahi?**

Recursion naturally hume ye order deta hai. Node ko **return/backtrack ke time** answer mein daalte hain.

So DFS ko Kahn ka `indegree--` mechanism nahi chahiye.

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
