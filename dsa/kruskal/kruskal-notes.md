# Kruskal — MST

## 🧠 Problem Feel

```text
Weighted + Undirected Graph
        ↓
All nodes connected
        ↓
Minimum total cost
        ↓
MST
```

### Kruskal ki feel

> **Edge-centric:** cheapest edges ko consider karo, but sirf useful edge accept karo.

```text
cheapest edge
      ↓
cycle banegi?
   YES → reject
   NO  → accept + union
```

---

# 🔥 How I reached it myself

### 1. Min-heap thought

Mera first thought tha:

> **Min-heap rakho taaki smallest weight wali edge pehle mile.**

Phir doubt aaya:

> **Cheapest edge blindly select kar sakte hain?**

No.

Agar endpoints already indirectly connected hain:

```text
A ── B ── C
```

Aur `A-C` edge aaye, toh add karne se cycle banegi.

---

### 2. Important observation

```text
find(u) == find(v)
        ↓
 same component
        ↓
 new edge → cycle
        ↓
 reject
```

```text
find(u) != find(v)
        ↓
 different components
        ↓
 edge useful
        ↓
 accept + union
```

### ⭐ Key thing I made myself

> **Candidate edge aur selected edge same cheez nahi hain.**

Cheapest edge sirf candidate hai; selected tab hogi jab woh connectivity increase kare aur cycle na banaye.

---

# 🔗 DSU + Kruskal Connection

DSU ka role:

> **Cycle detect karna + components merge karna.**

Kruskal ka role:

> **Edges ko cheapest → largest order mein process karna.**

```text
Kruskal
   ↓
choose edge order
   ↓
DSU
   ↓
same component?
   ↓
cycle → reject
otherwise → merge
```

Important: original graph ki cycle nahi detect kar rahe. **Selected MST edges mein new edge add karne se cycle banegi ya nahi**, ye check kar rahe hain.

---

# N Nodes → N-1 MST Edges

```text
1 node → 0 edges
2 nodes → 1 edge
3 nodes → 2 edges
4 nodes → 3 edges
```

> **MST for N nodes always has exactly N-1 edges.**

Isliye `edgesUsed == n - 1` par stop kar sakte hain.

---

# 🧠 When should Kruskal feel natural?

```text
Minimum total connection cost
+
all vertices/nodes connected
        ↓
       MST
```

Kruskal ki specific feel:

> **"Cheapest edges ko globally consider karun."**

Prim ki feel different hai: current connected component se cheapest outgoing edge choose karna.

---

# LeetCode 1584 — Special Observation

Normally graph mein edges given hote hain. 1584 mein **points diye hain, edges explicitly nahi**.

```text
Each point = node
Every pair = possible edge
```

For `i, j`:

```text
weight = |xi - xj| + |yi - yj|
```

So:

```text
possible edges = N(N-1)/2
```

Generate only `j = i + 1` onward because `i-j` and `j-i` same undirected edge hain.

Store:

```text
{u, v, weight}
```

Then normal Kruskal.

### My optimization thought

Mere dimaag mein aaya:

> **"Minimum hi chahiye, toh har edge store kyun karun? Current minimum variable/heap se kaam ho sakta hai?"**

Important realization:

> Sirf current minimum enough nahi hai; next-smallest candidates bhi repeatedly chahiye. Agar saari edges heap mein daal di, toh storage problem solve nahi hoti — edges phir bhi store hongi.

---

# 🔄 Cycle Detection: DFS/BFS vs DSU

### Generic graph cycle problem

```text
"Is graph mein cycle hai?"
        ↓
DFS/BFS is direct approach
```

Edge-list based situations mein DSU bhi useful hai.

### Kruskal

```text
"Kya new edge selected edges mein cycle banayegi?"
        ↓
find(u) == find(v)
```

So:

> **DFS/BFS = existing graph ki cycle check.**
>
> **DSU = edge processing ke time connectivity/cycle check.**

---

# ⏱️ Complexity

For `E` edges:

```text
Sort             O(E log E)
DSU operations   O(E α(N)) amortized
Overall          O(E log E)
Space            O(E + N)
```

For 1584, `E = O(N²)` because every pair can be an edge.

---

# 🧠 Final Mental Model

```text
Minimum cost + all connected
            ↓
           MST
            ↓
        Kruskal
            ↓
    cheapest edge first
            ↓
      same component?
       /          \
     YES           NO
      ↓             ↓
   reject       accept + union
                     ↓
                  next edge
```

> **Kruskal = cheapest useful edge.**
>
> Useful = **two different components ko connect kare.**
