# Bipartite Graph

## 🧠 Core Intuition

Bipartite graph ko simple way mein aise socho:

> Graph ke nodes ko 2 groups / colors mein divide karna hai, aur har edge ke dono ends different groups mein hone chahiye.

Hum 2 colors use kar sakte hain. Colors ki actual value important nahi hai — bas 2 different states chahiye.

---

## 🔥 Main Logic — Runtime Par Color Change

Meri initial mistake thi ki `1` aur `2` ko hardcode karke manually change kar raha tha.

Correct approach:

> Current node ka color jo hai, neighbor ko uska **opposite color** do.

### Agar colors `1` aur `2` hain

Agar:

```text
current = 1
```

to opposite:

```text
2
```

Aur agar:

```text
current = 2
```

to opposite:

```text
1
```

Isko runtime par automatically karne ka simple trick:

```text
opposite = 3 - currentColor
```

Because:

```text
3 - 1 = 2
3 - 2 = 1
```

So `1 -> 2` aur `2 -> 1` automatically ho jayega.

### C++

```cpp
color[v] = 3 - color[u];
```

### Java

```java
color[v] = 3 - color[u];
```

---

## 🔄 Alternative — Colors `0` and `1`

Agar hum colors `0` aur `1` use karte hain, to opposite color ka formula hoga:

```text
opposite = 1 - currentColor
```

Because:

```text
1 - 0 = 1
1 - 1 = 0
```

So dono approaches same concept represent karti hain:

```text
Colors 1,2  ->  3 - currentColor
Colors 0,1  ->  1 - currentColor
```

### 💡 Important

Formula color numbering par depend karta hai. **Concept hamesha same hai: neighbor ko opposite color do.**

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

Agar colors `1` aur `2` hain:

```text
color[start] = 1
```

Phir uske neighbor ko:

```text
color[neighbor] = 3 - color[current]
```

do.

Example:

```text
current = 1  -> neighbor = 2
current = 2  -> neighbor = 1
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

Agar colors `1` and `2` hain:

```text
opposite = 3 - currentColor
```

Ye cleaner hai aur runtime par automatically `1 -> 2` aur `2 -> 1` handle karta hai.

Agar colors `0` and `1` hain:

```text
opposite = 1 - currentColor
```

---

## 🧩 C++ Core Pattern — Colors `1` and `2`

```cpp
color[start] = 1;

if (color[v] == -1) {
    color[v] = 3 - color[u];
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

## ☕ Java Core Pattern — Colors `1` and `2`

```java
color[start] = 1;

if (color[v] == -1) {
    color[v] = 3 - color[u];
} else if (color[v] == color[u]) {
    return false;
}
```

Same idea. Language change hui hai, algorithm nahi.

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

Colors 1,2:
    Start node -> color 1
    Neighbor   -> 3 - currentColor

Colors 0,1:
    Start node -> color 0
    Neighbor   -> 1 - currentColor

Already colored neighbor:
    same color  -> NOT bipartite
    different   -> continue
```

### 🔥 One-line trick

> **Color number yaad mat rakho; bas opposite color do. 1/2 ke liye `3 - currentColor`, 0/1 ke liye `1 - currentColor`.**
