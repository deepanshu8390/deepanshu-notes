# Kruskal — MST

## 🧠 Problem / Feel

Graph mein:

- nodes connected hone chahiye
- total edge cost **minimum** chahiye

Ye **MST (Minimum Spanning Tree)** wali smell hai.

### Kruskal ki feel

> **Edge-centric thinking:** cheapest edges ko consider karo aur useful edge ko accept karo.

```text
Weighted undirected graph
        ↓
Minimum total cost to connect all nodes
        ↓
MST
        ↓
Kruskal / Prim
```

Kruskal mein natural thought:

```text
cheapest edge
    ↓
useful hai?
    ↓
cycle nahi banani
    ↓
accept + merge
```

---

## 🔥 How I reached Kruskal myself

Initially thought:

> Min-heap rakh sakte hain jisse smallest edge pehle mile.

Then important doubt:

> **Cheapest edge blindly select nahi kar sakte.**

Agar dono endpoints already indirectly connected hain, edge add karne se cycle banegi.

Example:

```text
A ── B ── C
```

Ab `A-C` edge cheap ho sakti hai, but A aur C already connected hain.

So:

```text
find(A) == find(C)
        ↓
   same component
        ↓
     cycle
        ↓
     reject
```

Otherwise:

```text
find(A) != find(C)
        ↓
 different components
        ↓
      accept
        ↓
      union
```

### 💡 Key connection I made

DSU ka role Kruskal mein:

> **Cycle detect karna + components merge karna.**

Ye wahi DSU hai jo pehle seekha tha; Kruskal bas decide karta hai ki kaunsi edges process/merge karni hain.

---

## 🔑 Kruskal core logic

```text
All edges
    ↓
sort by weight
    ↓
smallest → largest
    ↓
find(u), find(v)
    ↓
same root?
  YES → reject (cycle)
  NO  → accept + union
```

### Important

**Original graph mein cycle detect nahi kar rahe.**

Hum check kar rahe hain:

> **Selected MST edges mein new edge add karne se cycle banegi ya nahi?**

---

## N nodes → N-1 MST edges

```text
1 node → 0 edges
2 nodes → 1 edge
3 nodes → 2 edges
4 nodes → 3 edges
```

So:

> **MST for N nodes always has exactly N-1 edges.**

Isliye implementation mein `edgesUsed == n - 1` par stop kar sakte hain.

---

# LeetCode 1584 — Min Cost to Connect All Points

### Important twist

Previous MST problems mein edges + weights already diye ho sakte hain.

1584 mein **points diye hain, edges explicitly nahi diye**.

```text
Each point = node
Every pair of points = possible edge
```

For points `i` and `j`:

```text
weight = |xi - xj| + |yi - yj|
```

So N points ke liye:

```text
N choose 2 = N(N-1)/2
```

possible edges.

Generate only `j = i + 1` onward because:

```text
0-1 == 1-0
```

same undirected edge hai.

Store each edge as:

```text
{u, v, weight}
```

Then normal Kruskal:

```text
all pairs
   ↓
calculate Manhattan distance
   ↓
store {u,v,weight}
   ↓
sort by weight
   ↓
DSU
```

### My optimization thought

Mere dimaag mein aaya:

> "Minimum hi chahiye, toh kya har edge ka weight store karna zaroori hai? Current minimum variable ya heap se kaam ho sakta hai?"

Important realization:

> **Sirf current minimum enough nahi hai; Kruskal ko next-smallest valid candidates bhi repeatedly chahiye.**

Agar saari `N(N-1)/2` edges heap mein daal di, toh memory saving nahi hui; edges phir bhi store ho rahe hain. Standard Kruskal mein sorting simpler hai.

---

## Cycle detection: DFS/BFS vs DSU

### Generic graph cycle problem

> "Is this graph cyclic?"

Undirected graph ke liye DFS/BFS standard approach hai. Edge-list based problems mein DSU bhi useful hai.

### Kruskal

> "Kya ye new edge selected edges mein cycle banayegi?"

DSU naturally answer karta hai:

```text
find(u) == find(v) → cycle → reject
find(u) != find(v) → safe → union
```

So:

> **DFS/BFS = existing graph ki cycle check.**
>
> **DSU = edges process karte waqt connectivity/cycle check.**

---

## ⏱️ Complexity

For standard Kruskal with `E` edges:

```text
Sort edges       → O(E log E)
DSU operations   → O(E α(N)) amortized

Overall          → O(E log E)
Space            → O(E + N)
```

For LeetCode 1584, complete graph has:

```text
E = N(N-1)/2 = O(N²)
```

so generating/storing all pair edges itself takes `O(N²)` space, and sorting dominates with `O(N² log N)`.

---

## 🧠 Final mental model

```text
Minimum cost + all nodes connected
            ↓
           MST
            ↓
        Kruskal
            ↓
    cheapest edge first
            ↓
      cycle banegi?
       /          \
     YES           NO
      ↓             ↓
   reject       accept + union
                     ↓
                  next edge
```

### Key trick

> **Kruskal = cheapest useful edge.**
>
> Useful means: **it connects two different components.**
