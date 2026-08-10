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

## 🔥 Ab actual problem kya hai?

Hume baar-baar poocha ja sakta hai:

> **Kya 1 aur 4 same group / component mein hain?**

Answer:

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

### 💡 Bas yahi DSU ki starting problem hai

> **Groups/components maintain karne hain — kaun kis group mein hai aur do groups ko ek karna hai.**

Abhi `parent`, `find`, `union`, `rank` kuch nahi. Ye sab isi problem ko efficiently solve karne ki zarurat se naturally derive karenge.
