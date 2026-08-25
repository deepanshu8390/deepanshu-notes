# Flyweight Design Pattern

## Core problem

Flyweight ka problem **memory duplication** hai.

Suppose game mein 10,000 trees hain. Har tree ke paas:

```text
texture
color
x
 y
```

Agar `texture` aur `color` sab trees ke liye same hain, toh same data ko 10,000 baar store karna waste hai.

```text
Tree 1 → oak.png + green
Tree 2 → oak.png + green
Tree 3 → oak.png + green
...
Tree 10000 → oak.png + green
```

### Flyweight ka solution

> **Jo data objects ke beech same hai, uski ek shared copy rakho aur multiple objects ko usi object ka pointer/reference do.**

```text
Tree1 ──┐
Tree2 ──┼──→ ONE shared TreeType
Tree3 ──┘
```

---

## Intrinsic vs Extrinsic State

Flyweight samajhne ka sabse important concept.

### Intrinsic state

Jo data **same/shareable** hai.

Example:

```text
texture
color
Tree type
```

Isko Flyweight object mein rakhte hain.

### Extrinsic state

Jo data **har object ke liye different** hai.

Example:

```text
x
 y
position
```

Isko actual individual object mein rakhte hain.

So:

```text
Tree
├── x          ← extrinsic / unique
├── y          ← extrinsic / unique
└── type*      ← pointer to shared intrinsic data
```

---

## Simple C++ example

Shared/common data:

```cpp
class TreeType {
    string texture;
    string color;

public:
    TreeType(string texture, string color) {
        this->texture = texture;
        this->color = color;
    }
};
```

`this->texture` ka matlab:

```text
this->texture → current object ka member variable
texture       → constructor ka parameter
```

So:

```cpp
this->texture = texture;
```

means:

> Current `TreeType` object ka `texture` member = client se aayi constructor value.

Same for color.

Initializer-list version bhi possible hai:

```cpp
TreeType(string texture, string color)
    : texture(texture), color(color)
{}
```

Yahan `:` member initializer list start karta hai, aur `texture(texture)` ka meaning hai **member `texture` ko parameter `texture` se initialize karo**.

Learning ke liye `this->` version simpler hai; pattern ke liye dono equivalent intent dikhate hain.

---

## Actual Tree object

```cpp
class Tree {
    int x;
    int y;
    TreeType* type;

public:
    Tree(int x, int y, TreeType* type) {
        this->x = x;
        this->y = y;
        this->type = type;
    }
};
```

Ab ek shared `TreeType` object:

```cpp
TreeType oak("oak.png", "green");
```

Aur multiple trees:

```cpp
Tree t1(10, 20, &oak);
Tree t2(50, 80, &oak);
Tree t3(100, 200, &oak);
```

Visualization:

```text
                 oak
          ┌─────────────────┐
          │ texture=oak.png │
          │ color=green     │
          └─────────────────┘
             ↑      ↑      ↑
             │      │      │
            t1     t2     t3
          x=10    x=50   x=100
          y=20    y=80   y=200
```

### Important insight

Client `Tree` banate time **texture/color dobara nahi de raha**.

Client sirf:

```text
x
 y
pointer/reference → shared TreeType
```

deta hai.

For example:

```cpp
Tree t1(10, 20, &oak);
```

`&oak` ka meaning:

> Is Tree ka apna `TreeType` mat banao. Already existing `oak` object ko use karo.

This is the actual Flyweight point.

---

## Why pointer/reference?

Agar har Tree mein common data copy kar dein:

```text
Tree1 → own texture + color
Tree2 → own texture + color
Tree3 → own texture + color
```

memory duplicate hogi.

Instead:

```text
Tree1 ──┐
Tree2 ──┼──→ ONE TreeType object
Tree3 ──┘
```

Ek `TreeType*` enough hai because `texture` aur `color` dono `TreeType` ke andar hain.

```text
Tree
 ├── x
 ├── y
 └── type ─────→ TreeType
                    ├── texture
                    └── color
```

Agar shared data ko modify nahi karna ho, pointer ko `const TreeType*` bhi bana sakte hain.

---

## Flyweight Factory

Agar client har baar khud ye kare:

```cpp
new TreeType("oak.png", "green");
```

toh sharing ka purpose khatam ho sakta hai.

Isliye Factory ensure karti hai ki same type ka same Flyweight reuse ho.

Conceptually:

```text
Client
  ↓
getTreeType("oak")
  ↓
Factory
  ↓
Already exists?
  ├── YES → same TreeType return
  └── NO  → create, store, return
```

Simple idea:

```cpp
TreeType* oak = factory.getTreeType("oak");
```

Client ko bas bolna hai:

> Mujhe Oak type chahiye.

Factory ka kaam:

> Oak pehle se hai toh wahi shared object do, nahi hai toh ek baar banao.

---

## Flyweight vs Template Method

Dono mein **reuse** hota hai, but problem completely different hai.

### Template Method

Problem:

> **Algorithm same hai, kuch steps customizable hain.**

```text
SAME ALGORITHM
+
SOME STEPS CUSTOMIZABLE
```

Example:

```text
boilWater()
custom step
pour()
```

Tea aur Coffee mein skeleton same, kuch steps different.

### Flyweight

Problem:

> **Bahut saare objects hain aur unke andar common data duplicate ho raha hai.**

```text
MANY OBJECTS
+
SAME DATA
↓
SHARE THAT DATA
```

Flyweight ka concern algorithm ya overriding nahi hai. **Memory optimization through sharing** hai.

---

## Flyweight vs Singleton

### Singleton

Application mein ek hi instance:

```text
A ──┐
B ──┼──→ ONE object
C ──┘
```

### Flyweight

Multiple shared Flyweights ho sakte hain:

```text
Tree1 ──┐
Tree2 ──┼──→ Oak Flyweight
Tree3 ──┘

Car1 ──┐
Car2 ──┼──→ Car Flyweight
Car3 ──┘
```

Flyweight ka goal **one object globally** nahi, balki **duplicate common state ko share karke memory bachana** hai.

---

## Key memory line

**Flyweight = Same data ki 10,000 copies mat rakho; ek shared copy rakho aur sab objects ko uska pointer/reference do.**

Most important terms:

```text
Intrinsic state  = shared/common
Extrinsic state  = per-object/unique
```

Core structure:

```text
Many Objects
     ↓
shared Flyweight data
```
