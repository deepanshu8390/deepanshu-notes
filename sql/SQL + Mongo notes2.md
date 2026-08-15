# SQL + Mongo Notes 2

# SQL — JOINs

## 1. JOIN ka actual purpose

> **JOIN = alag tables/rows ki related information ko ek result mein lana.**

Example:

```text
employees.department_id
        ↓
departments.id
        ↓
departments.department
```

Agar employees mein sirf `department_id = 10` hai aur department ka actual naam `IT` doosri table mein hai, JOIN dono ki information combine karta hai.

---

## 2. `ON` — JOIN ka connection rule

```sql
FROM employees e
JOIN departments d
ON e.department_id = d.id;
```

`ON` batata hai:

> **Kaunsi 2 rows ka pair banana hai?**

Conceptually:

```text
left row + right row
        ↓
ON condition
        ↓
TRUE  → pair result mein
FALSE → pair reject
```

### 🔥 IMPORTANT: `ON` mein MULTIPLE CONDITIONS aa sakti hain

`ON` sirf ek condition nahi leta.

```sql
ON p1.category = p2.category
AND p1.price < p2.price
```

Dono conditions pair ke liye TRUE honi chahiye.

```text
ON condition 1
AND condition 2
AND condition 3
```

---

## 3. `ON` mein sirf `=` nahi hota

Different comparison operators use ho sakte hain:

```sql
ON a.id = b.id
ON a.id > b.id
ON a.id < b.id
ON a.id >= b.id
ON a.id <= b.id
ON a.id != b.id
```

`=` normal relational/key JOIN mein sabse common hai.

### Operator order

```text
=   → a = b and b = a same logical condition
!=  → a != b and b != a same logical condition

>   → order matters
<   → order matters
>=  → order matters
<=  → order matters
```

Example:

```text
30 > 20 → TRUE
20 > 30 → FALSE
```

---

# INNER JOIN

```sql
SELECT e.name, d.department
FROM employees e
JOIN departments d
ON e.department_id = d.id;
```

`JOIN` by itself normally means `INNER JOIN`.

### Feel

```text
Only matching pairs

match mila  → result mein
match nahi  → result se gone
```

If employee has `department_id = 40` but departments table has no `id = 40`, INNER JOIN mein that employee row nahi aayegi.

---

# LEFT JOIN

```sql
SELECT e.name, d.department
FROM employees e
LEFT JOIN departments d
ON e.department_id = d.id;
```

Meaning:

> **LEFT table ki saari rows rakho; right se match mila toh information jodo, nahi mila toh NULL.**

```text
LEFT table → ALL rows
RIGHT table → matching data / NULL
```

Example:

```text
employees:
Priya | department_id = 40

departments:
10 | IT
20 | HR
30 | Sales
```

No `40` match.

LEFT JOIN result:

```text
Priya | NULL
```

---

# RIGHT JOIN

```sql
SELECT e.name, d.department
FROM employees e
RIGHT JOIN departments d
ON e.department_id = d.id;
```

Meaning:

> **RIGHT table ki saari rows rakho; left se match mila toh information jodo, nahi mila toh NULL.**

```text
RIGHT table → ALL rows
LEFT table  → matching data / NULL
```

If `Sales` department exists but no employee belongs to it:

```text
NULL | Sales
```

### LEFT vs RIGHT

```text
LEFT JOIN
→ left ki all rows

RIGHT JOIN
→ right ki all rows

INNER JOIN
→ only matched pairs
```

LEFT aur RIGHT logically mirror hain. Often LEFT JOIN likhna easier hota hai because main table ko left side par rakhna natural lagta hai.

---

# 🔥 ON vs WHERE — Very Important

```text
ON
→ JOIN ke time decide karta hai
  kaunsi 2 rows ka pair banega

WHERE
→ JOIN ke baad bane result ko filter karta hai
```

### Example

```sql
FROM products p1
JOIN products p2
ON p1.category = p2.category
AND p1.price < p2.price
```

Here `ON` decides valid product pairs.

Then:

```sql
WHERE p1.price > 1000
```

means:

> Join se jo pairs ban gaye, unmein se final result mein kaunse rakhne hain.

### Don't memorize it as "one condition vs another"

Difference is **role**:

```text
ON    → pair creation / JOIN condition
WHERE → final result filtering
```

---

# SELF JOIN 🔥

## Self JOIN ka actual reason

Same table mein ek row ke andar **doosri row ka reference/ID** ho aur hume us doosri row ki information chahiye.

Example:

```text
id | name  | manager_id
-----------------------
1  | Rahul | NULL
2  | Aman  | 1
3  | Priya | 1
4  | Rohit | 2
```

Aman ki row:

```text
Aman | manager_id = 1
```

Manager ka name Aman ki row mein nahi hai.

So same `employees` table ko dobara dekhna padega:

```text
Aman.manager_id = 1
        ↓
employees.id = 1
        ↓
Rahul ki row
        ↓
Rahul.name
```

### Self JOIN

```sql
SELECT e.name AS employee,
       m.name AS manager
FROM employees e
JOIN employees m
ON e.manager_id = m.id;
```

Same table, two roles:

```text
e = employee role
m = manager role
```

### Most important mental model

> **Self JOIN = ek row se value/reference nikalo → usi value se same table ki doosri row find karo.**

`ON e.manager_id = m.id` ko read karo:

> Employee ki `manager_id` ko same employees table ke `id` se match karo.

Manager ka name **match nahi** ho raha. `ID` se row find ho rahi hai; `m.name` baad mein output mein return ho raha hai.

---

# WHERE vs Self JOIN

```sql
WHERE name = 'Rahul'
```

→ same table ki existing rows ko filter karke Rahul ki row nikalta hai.

But:

```text
Aman.manager_id = 1
```

→ hume `id = 1` wali **doosri row** chahiye.

So:

```text
WHERE
→ existing rows FILTER karta hai

JOIN
→ doosri row/table ki INFORMATION bhi LAATA hai
```

This is why self JOIN is needed.

---

# Self JOIN — Different Example

Products:

```text
id | name     | price | category
--------------------------------
1  | Mouse    | 500   | Electronics
2  | Keyboard | 1200  | Electronics
3  | Monitor  | 8000  | Electronics
4  | Chair    | 5000  | Furniture
5  | Desk     | 9000  | Furniture
```

Question:

> Find pairs of products from the same category where the first product is cheaper than the second.

```sql
SELECT p1.name, p2.name
FROM products p1
JOIN products p2
ON p1.category = p2.category
AND p1.price < p2.price;
```

### Why no GROUP BY?

Because we need **pairs**, not groups.

```text
GROUP BY
→ rows ko groups mein collect/collapse karta hai

JOIN ON
→ valid row-pairs banata hai
```

Example:

```text
Mouse | Electronics | 500
Keyboard | Electronics | 1200
```

```text
Electronics = Electronics  → TRUE
500 < 1200                 → TRUE
```

So:

```text
Mouse | Keyboard
```

But:

```text
Mouse | Electronics | 500
Chair | Furniture   | 5000
```

```text
Electronics = Furniture → FALSE
```

Pair reject.

---

# JOIN + WHERE

Example:

```sql
SELECT e.name
FROM employees e
JOIN departments d
ON e.department_id = d.id
WHERE d.department = 'IT'
AND e.salary > 70000;
```

Mental flow:

```text
employees
    ↓
JOIN departments
    ↓
ON → related rows/pairs
    ↓
WHERE → filter resulting data
    ↓
SELECT → required output
```

---

# JOIN Performance — Intuition Only

Conceptually, for `M` left rows and `N` right rows, JOIN can be thought of as potentially checking many row pairs:

```text
M × N
```

This feels like a nested-loop C++ solution.

But **real databases do not necessarily brute-force every pair**. Query optimizers can choose execution strategies such as index-based lookup, nested-loop join, hash join, merge join, etc., depending on the database, indexes, statistics and query.

Important distinction:

```text
Logical JOIN meaning
≠
Actual physical execution algorithm
```

For learning JOINs, first understand **which row-pairs satisfy `ON`**. Optimization comes later.

---

# 🧠 JOIN Ultimate Mental Model

```text
JOIN
 ↓
2 tables / 2 roles ki rows
 ↓
ON condition
 ↓
valid row-pairs
 ↓
SELECT → kya information dikhani hai?
 ↓
WHERE → final result mein kya filter karna hai?
```

### Core memory

```text
ON
→ kaunsi rows ka PAIR banega?

WHERE
→ bane hue result mein KIN rows ko rakhna hai?

INNER
→ only matching pairs

LEFT
→ left ki all rows + right match/NULL

RIGHT
→ right ki all rows + left match/NULL

SELF JOIN
→ same table ko 2 roles mein use karke
   ek row se reference lekar doosri row find karna
```
