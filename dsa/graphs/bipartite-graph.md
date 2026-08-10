# Bipartite Graph

## 🧠 Basic Intuition — problem tak kaise pahuche

Bipartite ka simple meaning:

> Graph ke nodes ko **2 groups / colors** mein divide karna hai, aur har connected pair different group mein hona chahiye.

To naturally hum sochte hain:

```text
2 groups
   ↓
2 colors
   ↓
Current node ko ek color
Neighbor ko opposite color
```

Bas isi se BFS/DFS coloring approach aa jaati hai.

---

## 🔥 Key Logic / Observation

Agar colors `1` aur `2` use kar rahe hain:

```text
1 → 2
2 → 1
```

Isko hardcode karne ki zarurat nahi.

Runtime par:

```cpp
nextColor = 3 - currentColor;
```

Because:

```text
3 - 1 = 2
3 - 2 = 1
```

Agar colors `0` aur `1` ho, then:

```cpp
nextColor = 1 - currentColor;
```

### Main condition

Har edge ke liye:

```text
color[u] != color[v]
```

Agar already-colored neighbor ka color same mil gaya → **not bipartite**.

---

## ⚠️ My Coding Mistake

Maine `1` aur `2` ko hardcode karke transition likh diya tha:

```text
1 → 2
2 → 1
```

Problem ye nahi tha ki answer galat aa raha tha; problem ye thi ki main **color ko actual value samajh raha tha**.

Color bas ek **state** hai.

Better mental model:

> Current color jo bhi hai, neighbor ko uska opposite color do.

Isliye runtime calculation:

```cpp
3 - currentColor
```

---

## 🔁 BFS / DFS — Bas Pattern Yaad Rakho

Dono mein core logic same hai:

```text
uncolored node
      ↓
assign a color
      ↓
visit neighbours
      ↓
uncolored → opposite color
already colored → check different?
```

BFS ya DFS sirf **traversal ka method** change karta hai; bipartite ka coloring logic same rehta hai.

---

## 💡 Things to Revise

- Bipartite = **2-colorable graph**
- `color[u] != color[v]`
- Neighbor ko **opposite color** dena hai
- `1,2` colors → `3 - currentColor`
- `0,1` colors → `1 - currentColor`
- BFS/DFS dono mein same coloring idea
- `-1` generally means **abhi color assign nahi hua**
