# DSU (Disjoint Set Union)

## 🧠 Sabse pehle problem samjho

Maan lo 5 log hain:

```text
1   2   3   4   5
```

Abhi koi connected nahi hai.

```text
{1}   {2}   {3}   {4}   {5}
```

Har banda apne **alag group / component** mein hai.

---

### Connection 1

```text
1 ---- 2
```

Ab:

```text
{1,2}   {3}   {4}   {5}
```

### Connection 2

```text
3 ---- 4
```

Ab:

```text
{1,2}   {3,4}   {5}
```

### Connection 3

```text
2 ---- 3
```

Dhyaan do:

`2` already `{1,2}` mein hai.

`3` already `{3,4}` mein hai.

Toh connection dono groups ko ek kar dega:

```text
{1,2} ---- {3,4}
```

Ab final groups:

```text
{1,2,3,4}   {5}
```

---

## 🔥 Actual problem kya hai?

Hume baar-baar poocha ja sakta hai:

> **Kya 1 aur 4 same group / component mein hain?**

```text
YES
```

Aur:

> **Kya 1 aur 5 same group mein hain?**

```text
NO
```

Saath hi naye connections bhi aate rahenge:

```text
"2 aur 5 ko connect karo"
```

Toh hume baar-baar do kaam karne hain:

```text
1. Pata karo → ye banda kis group mein hai?
2. Do groups ko ek karo.
```

### 💡 Starting intuition

> **Groups/components maintain karne hain — kaun kis group mein hai aur do groups ko ek karna hai.**

---

# 🧠 Pehla thought — list bana doon?

Natural thought aa sakta hai:

```text
Group A = {1,2,3,4}
Group B = {16,17}
```

Aur merge ke time ek nayi list bana doon:

```text
{1,2,3,4,16,17}
```

### ⚠️ Problem

Agar baar-baar groups merge karne hain, toh elements ko physically copy/move karna costly ho sakta hai.

CP wali thinking:

> **Data ko physically merge kyun karun? Kya representation ko hi change kar sakta hoon?**

---

# 🔗 Linked List wali thinking

Ek aur natural representation:

> Har node ke paas uska `parent` pointer/reference ho.

Example:

```text
    1
    ↑
    2
    ↑
    3
    ↑
    4
```

Meaning:

```text
parent[4] = 3
parent[3] = 2
parent[2] = 1
parent[1] = 1
```

Agar **4 ka node/reference already mere paas hai**, toh `parent[4]` direct mil jayega. Kisi list ko traverse karke parent search nahi karna padega.

Isliye actual important problem ye nahi hai:

> "Parent ko locate kaise karun?"

Actual problem hai:

> **"Parent ke through root tak kitna travel karna padega?"**

Isi parent-pointer structure ko hum practically array/vector mein store karte hain:

```text
parent[x]
```

---

# 👑 Representative / Root

Har group ka ek **representative/head/root** rakhte hain.

Example:

```text
    1
   / \
  2   3
```

```text
parent[1] = 1
parent[2] = 1
parent[3] = 1
```

Rule:

> **Jiska `parent[x] == x`, wahi root/representative hai.**

Initially:

```text
1   2   3
```

Sab apne-apne root hain:

```text
parent[1] = 1
parent[2] = 2
parent[3] = 3
```

Aur initially har component ka size `1` hai.

### ⚠️ Important distinction

```text
parent[x] == x
        ↓
      x is ROOT

size[x]
        ↓
root ke poore component mein total nodes
```

`find(x) == x` ka matlab **root** hai; iska matlab size hamesha `1` nahi hai. Size `1` sirf initial single-node component mein hota hai.

---

# 🔎 Find — group ka head dhoondo

`find(x)` ka simple meaning:

> **x ke group ka actual representative/root dhoondo.**

Example:

```text
    1
   / \
  2   3
```

### `find(1)`

```text
parent[1] == 1
```

So:

```text
find(1) = 1
```

### `find(3)`

```text
3 → 1
```

Then:

```text
parent[1] == 1
```

So:

```text
find(3) = 1
```

Longer chain:

```text
    1
    ↑
    2
    ↑
    3
    ↑
    4
```

```text
find(4)
4 → 3 → 2 → 1
            ↑
          root
```

### Recursive intuition

```text
Agar main root hoon → mujhe return karo.
Warna → mere parent ka root dhoondo.
```

So simple recursive `find`:

```cpp
int find(int x) {
    if (parent[x] == x)
        return x;

    return find(parent[x]);
}
```

**Abhi path compression intentionally nahi hai.**

---

# 🔗 Do groups ko merge kaise karein?

Suppose:

```text
      1                 4
     / \               / \
    2   3             5   6
```

Connection aaya:

```text
2 ↔ 5
```

Direct `2` ko `5` se connect nahi karna.

Pehle actual roots:

```text
find(2) → 1
find(5) → 4
```

So actual groups hain:

```text
{1,2,3}       {4,5,6}
```

Ab roots `1` aur `4` ko connect karna hai.

For example:

```text
        1
      / | \
     2  3  4
           / \
          5   6
```

Bas **ek pointer change** se do groups logically ek ho gaye:

```cpp
parent[4] = 1;
```

> **Data ko physically merge nahi kiya; representation ko merge kiya.**

---

# 📏 Problem — random merge se tree lamba ho sakta hai

Agar hum randomly roots ko attach karte rahe:

```text
1
↑
2
↑
3
↑
4
↑
5
↑
6
```

Toh:

```text
find(6)
```

ko poora chain travel karna padega.

So next question:

> **Do trees merge karte waqt kaunsa root parent banana chahiye?**

---

# 💡 Observation — chhota tree bade tree ke neeche

Example:

```text
Tree A                 Tree B

    1                     16
   / \                    / \
  2   3                  17  18
 / \
4   5
```

Sizes:

```text
size[1]  = 5
size[16] = 3
```

So:

> **Jiske poore component mein zyada nodes hain, woh parent/baap banega.**

```text
parent[16] = 1
size[1] += size[16]
```

Result:

```text
          1
       / /|\ \
      2 3 4 5 16
             / \
            17 18
```

New size:

```text
size[1] = 8
```

### 🧠 Trick to remember

> **Parent = baap. Jiske poore component mein zyada nodes, woh baap.**

⚠️ "Zyada direct children" nahi — **poore component ke total nodes**.

---

# 🔥 Union by Size — derived logic

```text
find(a) → rootA
find(b) → rootB

rootA == rootB?
    YES → already same component

NO:
    bigger size wala → parent/baap
    smaller root → uske neeche
    bigger root ka size += smaller root ka size
```

Important:

> **Node ko nahi, root ko root ke neeche attach karna hai.**

```text
nodeA → find → rootA
nodeB → find → rootB

rootB → rootA
```

---

# 💻 C++ — Simple Find + Union by Size

```cpp
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

        return find(parent[x]);
    }

    void unite(int a, int b) {
        int rootA = find(a);
        int rootB = find(b);

        if (rootA == rootB)
            return;

        if (size[rootA] >= size[rootB]) {
            parent[rootB] = rootA;
            size[rootA] += size[rootB];
        } else {
            parent[rootA] = rootB;
            size[rootB] += size[rootA];
        }
    }
};
```

---

# 💻 Java — Standard Array Version

DSU mein `n` usually pehle se known hota hai, isliye standard Java implementation mein `int[]` natural choice hai.

```java
class DisjointSet {

    int[] parent;
    int[] size;

    DisjointSet(int n) {
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

        return find(parent[x]);
    }

    void unionBySize(int u, int v) {
        int rootU = find(u);
        int rootV = find(v);

        if (rootU == rootV)
            return;

        if (size[rootU] >= size[rootV]) {
            parent[rootV] = rootU;
            size[rootU] += size[rootV];
        } else {
            parent[rootU] = rootV;
            size[rootV] += size[rootU];
        }
    }
}
```

### Java ArrayList mapping

`ArrayList` bhi valid hai:

```text
Array       → ArrayList
arr[i]      → arr.get(i)
arr[i] = x  → arr.set(i, x)
```

But DSU mein `n` known hone ki wajah se `int[]` simpler hai.

---

# ⏱️ Time & Space Complexity — Current Version

**Current version = Union by Size + simple recursive Find. Path Compression abhi nahi hai.**

Union by Size tree ki height ko `O(log N)` tak control karta hai.

### `find(x)`

Worst-case tree height:

```text
O(log N)
```

So:

```text
Time  = O(log N)
Stack = O(log N)
```

### `union(a, b)`

Union ke andar do `find()` calls hote hain:

```text
find(a) → O(log N)
find(b) → O(log N)
size compare + pointer change → O(1)
```

So overall:

```text
Time = O(log N)
```

### Space

```text
parent[] = O(N)
size[]   = O(N)
recursion stack = O(log N)
```

So auxiliary data structure space:

```text
O(N)
```

### ⚠️ Without Union by Size

Agar random merging se chain ban gayi:

```text
1
↑
2
↑
3
↑
...
↑
N
```

Then:

```text
find = O(N)
recursion stack = O(N)
```

**Union by Size ne isi problem ko `O(N)` se `O(log N)` tak improve kiya.**

---

# ⚠️ Current State — Abhi kya nahi padha

Abhi humne **path compression nahi lagaya**.

Current DSU:

```text
Find
  ↓
parent chain follow karo

Union
  ↓
find roots
  ↓
union by size
```

Next natural question:

> `find(6)` karte waqt agar main root tak pahunch hi gaya, toh next time 6 ko wahi lambi chain kyun traverse karni pade?

**Yahin se Path Compression naturally derive hoga.**
