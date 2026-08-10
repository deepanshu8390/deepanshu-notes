# Bipartite Graph

## 🧠 Core Intuition

Bipartite graph ko simple way mein aise socho:

> Graph ke nodes ko 2 groups / colors mein divide karna hai, aur har edge ke dono ends different groups mein hone chahiye.

Hum usually 2 colors use karte hain:

- Color `0`
- Color `1`

Actual number `1` aur `2` important nahi hai. Bas **2 different states** chahiye.

---

## 🔥 Main Logic — Runtime Par Color Change

Meri initial mistake thi ki `1` aur `2` ko hardcode karke manually change kar raha tha.

Correct approach:

> Current node ka color jo hai, neighbor ko uska **opposite color** do.

Agar:

```text
current = 0
neighbor = 1
```

to next:

```text
current = 1
neighbor = 0
```

Runtime par transition automatically ho sakta hai:

```cpp
color[v] = 1 - color[u];
```

Java mein bhi same logic:

```java
color[v] = 1 - color[u];
```

Isliye `1` aur `2` ko hardcode karke:

```text
1 -> 2
2 -> 1
```

likhne ki zarurat nahi.

---

## 💡 Important Observation

Bipartite check ka actual question hai:

> Kya graph ko 2 colors se aise color kar sakte hain ki har edge ke endpoints different colors ke hon?

Isliye har edge `(u, v)` ke liye:

```text
color[u] != color[v]
```

hona chahiye.

Agar kisi already-colored neighbor ke liye:

```text
color[u] == color[v]
```

mil gaya, graph bipartite nahi hai.

---

## 🔄 BFS / DFS Coloring Pattern

Uncolored node se start karo.

```text
color[start] = 0
```

Phir uske neighbors:

```text
color[neighbor] = 1 - color[current]
```

Phir next level par:

```text
1 -> 0
0 -> 1
```

So graph mein coloring automatically alternate hoti rahegi.

---

## ⚠️ My Mistake

### Wrong thinking

Maine colors ko directly hardcode kar diya:

```text
if current == 1:
    neighbor = 2
else:
    neighbor = 1
```

Ye logically kaam kar sakta hai, but unnecessary hardcoding hai.

### Better thinking

Color ko **state** samjho, actual value nahi.

```text
opposite = 1 - current
```

Ye cleaner hai aur code mein actual concept clearly dikhta hai:

> Neighbor ko current ka opposite color do.

---

## 🧩 C++ Core Pattern

```cpp
color[v] = 1 - color[u];
```

BFS/DFS mein jab `v` uncolored ho:

```cpp
if (color[v] == -1) {
    color[v] = 1 - color[u];
}
else if (color[v] == color[u]) {
    return false;
}
```

`-1` ka meaning:

```text
abhi color assign nahi hua
```

---

## ☕ Java Core Pattern

```java
color[v] = 1 - color[u];
```

Same idea. Language change hui hai, algorithm nahi.

```java
if (color[v] == -1) {
    color[v] = 1 - color[u];
} else if (color[v] == color[u]) {
    return false;
}
```

---

## 🎯 Interview Recognition

Agar question mein aaye:

- 2 groups mein divide karo
- 2 colors mein color karo
- Adjacent nodes different hone chahiye
- No two connected nodes same group mein

Immediately think:

```text
Bipartite Graph
        ↓
2-coloring
        ↓
BFS / DFS
        ↓
neighbor = opposite color
```

---

## 📝 Quick Revision

```text
Bipartite = 2-colorable graph

Start node -> color 0
Neighbor   -> 1 - currentColor

Already colored neighbor:
    same color  -> NOT bipartite
    different   -> continue
```

### One-line trick

> **Color number yaad mat rakho; bas opposite color do.**
