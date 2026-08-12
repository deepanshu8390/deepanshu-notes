# Java Syntaxes — DSA Quick Reference

Java mein C++ ke `vector`, normal array, aur basic collection syntax mix-up na ho, isliye ye quick reference.

## 1. Normal Array

### Create `n` size array

```java
int[] arr = new int[n];
```

Remember:

```text
int[] arr
  ↑    ↑
 type  variable name
```

So **wrong**:

```java
int[] arr = new arr[5];   // ❌
```

Correct:

```java
int[] arr = new int[5];   // ✅
```

### Access

```java
arr[0]
arr[3] = 10;
```

### Size

```java
arr.length
```

### Initialize all values with `-1`

Java array initially `0` se fill hota hai:

```java
int[] arr = new int[n];
```

To fill with `-1`:

```java
Arrays.fill(arr, -1);
```

Need import:

```java
import java.util.Arrays;
```

---

## 2. ArrayList — C++ `vector` jaisa

```java
ArrayList<Integer> arr = new ArrayList<>();
```

Need import:

```java
import java.util.ArrayList;
```

### Add element

```java
arr.add(10);
arr.add(20);
```

### Access

```java
arr.get(0);
```

### Size

```java
arr.size();
```

### Replace existing element

```java
arr.set(0, 50);
```

### Important: `set()` vs `add()`

```java
arr.add(-1);      // ✅ new element add/create
arr.set(2, -1);   // ✅ existing index 2 ko replace
```

Empty ArrayList:

```java
ArrayList<Integer> arr = new ArrayList<>();
```

is initially:

```text
[]
```

Therefore:

```java
arr.set(0, -1);   // ❌ index 0 exists hi nahi karta
```

To create `n` elements initialized with `-1`:

```java
ArrayList<Integer> arr = new ArrayList<>();

for (int i = 0; i < n; i++) {
    arr.add(-1);
}
```

Now:

```text
[-1, -1, -1, ...]
```

### Important trap

```java
ArrayList<Integer> arr = new ArrayList<>(n);
```

This **does NOT create `n` elements**. It only reserves internal capacity approximately for `n` elements.

So this is still invalid:

```java
ArrayList<Integer> arr = new ArrayList<>(n);
arr.set(0, -1);   // ❌
```

---

## 3. Array vs ArrayList — DSA Cheat Sheet

| Task | Array | ArrayList |
|---|---|---|
| Create | `int[] arr = new int[n];` | `ArrayList<Integer> arr = new ArrayList<>();` |
| Add/create element | Fixed slots already exist | `arr.add(x)` |
| Access | `arr[i]` | `arr.get(i)` |
| Update | `arr[i] = x` | `arr.set(i, x)` |
| Size | `arr.length` | `arr.size()` |
| Initialize all `-1` | `Arrays.fill(arr, -1)` | loop + `arr.add(-1)` |

## 4. Mental Shortcut

**Array → boxes already created**

```java
int[] arr = new int[n];
```

Think:

```text
[0][0][0][0][0]
```

**ArrayList → initially empty, `add()` se boxes/elements aayenge**

```java
ArrayList<Integer> arr = new ArrayList<>();
```

Think:

```text
[]
```

Then:

```java
arr.add(-1);
```

becomes:

```text
[-1]
```

### One-line memory trick

> `[]` = fixed-size array
>
> `ArrayList<>` = dynamic list / C++ `vector` type
>
> `add()` = new element
>
> `set()` = existing element replace
