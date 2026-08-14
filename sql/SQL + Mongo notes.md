# SQL + Mongo Notes

## SQL — Basic Filtering

### 1. SELECT + FROM

```sql
SELECT name, salary
FROM employees;
```

- `SELECT` → kaunse **columns** chahiye
- `FROM` → kis **table** se chahiye

Mental model:

> `SELECT` = columns choose karo  
> `FROM` = table choose karo

---

### 2. WHERE = Row Filter

```sql
SELECT name
FROM employees
WHERE city = 'Delhi';
```

Mental model:

> **Har row mein specific column ki value check karo.**

For `WHERE city = 'Delhi'`:

```text
Row 1 → city check → Delhi    → ✅
Row 2 → city check → Gurgaon  → ❌
Row 3 → city check → Delhi    → ✅
```

So:

> **WHERE = WHICH ROWS chahiye?**

And:

> **SELECT = WHICH COLUMNS dikhane hain?**

### Most important distinction

```text
WHERE  → rows filter
SELECT → columns select
```

---

## SQL — AND / OR

`WHERE` ke saath multiple conditions combine kar sakte hain.

### AND

```sql
SELECT name
FROM employees
WHERE department = 'IT'
AND city = 'Gurgaon';
```

Meaning:

```text
department = IT
        AND
city = Gurgaon
```

**Dono conditions TRUE honi chahiye.**

### OR

```sql
SELECT name
FROM employees
WHERE city = 'Delhi'
OR city = 'Gurgaon';
```

Meaning:

```text
city = Delhi
      OR
city = Gurgaon
```

**Koi ek condition TRUE ho → row select.**

---

# MongoDB — AND / OR

### Implicit AND

Mongo mein multiple fields same object mein likhne par implicit `AND` hota hai:

```javascript
{
  department: "IT",
  city: "Gurgaon"
}
```

Means:

```text
department = IT
AND
city = Gurgaon
```

SQL equivalent:

```sql
WHERE department = 'IT'
AND city = 'Gurgaon'
```

MongoDB query example:

```javascript
db.employees.find({
  department: "IT",
  city: "Gurgaon"
})
```

### Explicit AND

```javascript
{
  $and: [
    { department: "IT" },
    { city: "Gurgaon" }
  ]
}
```

Usually implicit AND is enough for simple conditions.

### OR

Mongo uses `$or`:

```javascript
{
  $or: [
    { city: "Delhi" },
    { city: "Gurgaon" }
  ]
}
```

SQL equivalent:

```sql
WHERE city = 'Delhi'
OR city = 'Gurgaon'
```

---

# SQL — WHERE Operators

## 3. IN = Multiple Exact Values

Agar same column ko multiple **exact values** ke against check karna ho, `IN` use karte hain.

```sql
SELECT name
FROM employees
WHERE department IN ('HR', 'IT');
```

Mental model:

> Har row mein `department` check karo → kya value `HR` ya `IT` mein se koi hai?

`IN` is basically a short form of multiple `OR` conditions:

```sql
WHERE department = 'HR'
   OR department = 'IT';
```

### MongoDB mapping

```javascript
db.employees.find({
  department: {
    $in: ["HR", "IT"]
  }
})
```

```text
SQL  → IN
Mongo → $in
```

---

## 4. BETWEEN = Range Check

Agar kisi numeric value ko ek range ke andar check karna ho:

```sql
SELECT name, salary
FROM employees
WHERE salary BETWEEN 45000 AND 60000;
```

Mental model:

> Har row mein `salary` check karo → kya `45000` se `60000` ke beech hai?

Important:

> `BETWEEN` **both endpoints include** karta hai.

So:

```text
50000 → included ✅
60000 → included ✅
```

Equivalent idea:

```sql
WHERE salary >= 45000
AND salary <= 60000;
```

### MongoDB mapping

Mongo mein `BETWEEN` ka direct operator nahi hai. Range ke liye `$gte` + `$lte` use karte hain:

```javascript
db.employees.find({
  salary: {
    $gte: 45000,
    $lte: 60000
  }
})
```

```text
SQL      → BETWEEN 45000 AND 60000
MongoDB  → $gte: 45000 + $lte: 60000
```

---

## 5. LIKE = Text Pattern Matching

`LIKE` ka use text ke **pattern** ko match karne ke liye hota hai.

Example: name `R` se start ho:

```sql
SELECT name, salary
FROM employees
WHERE name LIKE 'R%';
```

### `%` wildcard

`%` = zero ya more characters / us position par kuch bhi ho sakta hai.

```text
'R%'   → R se start
'%n'   → n pe end
'%a%'  → a kahin bhi present / contains a
```

Example:

```sql
SELECT name
FROM employees
WHERE name LIKE '%a%';
```

→ name mein kahin bhi `a` ho.

### Multiple LIKE conditions

Agar ek hi text column par multiple patterns ki conditions hain, har pattern ko complete condition banana padega:

```sql
SELECT name, salary
FROM employees
WHERE name LIKE 'R%'
AND name LIKE '%n';
```

Yahan:

```text
name LIKE 'R%'  → R se start
name LIKE '%n'  → n pe end
```

❌ Sirf `AND '%n'` valid condition nahi hai.

✅ `AND name LIKE '%n'`

---

### MongoDB mapping for LIKE

MongoDB mein SQL `LIKE` ka direct operator nahi hai. Pattern matching ke liye **regex** use karte hain.

SQL:

```sql
WHERE name LIKE 'R%';
```

MongoDB:

```javascript
db.employees.find({
  name: /^R/
})
```

### MongoDB Regex Tricks — Quick Cheat Sheet

```text
/^R/      → R se start
/n$/      → n pe end
/a/       → a kahin bhi contains
/^R.*n$/  → R se start AND n pe end
/^[A-Z]/  → capital letter se start
/^[0-9]/  → digit se start
```

### 🔥 Main Regex Trick

SQL ke `%` ko Mongo regex mein mentally map karne ke liye:

```text
^   → start
$   → end
.*  → beech mein kuch bhi / zero or more characters
```

Isliye:

```text
SQL LIKE 'R%'   → /^R/
SQL LIKE '%n'   → /n$/
SQL LIKE '%a%'  → /a/
SQL LIKE 'R%n'  → /^R.*n$/
```

Example: **R se start AND n pe end**:

```javascript
{ name: /^R.*n$/ }
```

Isko tod:

```text
^R  → R se start
.*  → beech mein kuch bhi
n$  → n pe end
```

### More Regex Examples

```javascript
// R se start
db.employees.find({ name: /^R/ })

// n pe end
db.employees.find({ name: /n$/ })

// name mein 'a' kahin bhi
db.employees.find({ name: /a/ })

// R se start aur n pe end
db.employees.find({ name: /^R.*n$/ })
```

```text
SQL      → LIKE
MongoDB  → Regex
```

---

# SQL — NULL & NOT Operators

## 🚨🚨🚨 DANGER ZONE: `NULL` 🚨🚨🚨

> **Ye SQL ka sabse common silly trap hai.**
>
> `NULL` ko `=` se check kiya → **GALAT.**
>
> Baar-baar yaad rakh: **`NULL` ke saath `IS NULL` / `IS NOT NULL` use hota hai.**

### `IS NULL`

`NULL` = value missing / unknown.

```sql
SELECT name
FROM employees
WHERE city IS NULL;
```

Mental model:

> Har row mein `city` check karo → jahan city ki value missing hai, woh row select.

❌ **DO NOT WRITE:**

```sql
WHERE city = NULL
```

✅ **WRITE:**

```sql
WHERE city IS NULL
```

### MongoDB mapping

```javascript
db.employees.find({
  city: null
})
```

---

## `IS NOT NULL`

> City ki value missing **nahi** honi chahiye.

```sql
SELECT name
FROM employees
WHERE city IS NOT NULL;
```

### MongoDB mapping

```javascript
db.employees.find({
  city: {
    $ne: null
  }
})
```

---

## `NOT IN`

`IN` ka opposite.

```sql
SELECT name
FROM employees
WHERE department NOT IN ('HR', 'IT');
```

> HR aur IT ko chhodkar baaki employees.

### MongoDB mapping

```javascript
db.employees.find({
  department: {
    $nin: ["HR", "IT"]
  }
})
```

```text
SQL   → NOT IN
Mongo → $nin
```

---

## `NOT LIKE`

`LIKE` ka opposite.

```sql
SELECT name
FROM employees
WHERE name NOT LIKE 'R%';
```

> R se start hone wale names ko exclude karo.

### MongoDB mapping

```javascript
db.employees.find({
  name: {
    $not: /^R/
  }
})
```

```text
SQL   → NOT LIKE
Mongo → $not + regex
```

---

# SQL — Sorting & Limiting

## 6. ORDER BY = Sorting

`WHERE` ke baad hum rows ko **arrange/sort** kar sakte hain.

```sql
SELECT name, salary
FROM employees
ORDER BY salary ASC;
```

- `ASC` → ascending → small → large
- `DESC` → descending → large → small

Example:

```sql
ORDER BY salary DESC;
```

→ highest salary pehle.

### Mental model

```text
WHERE     → kaunsi rows?
ORDER BY  → kis order mein?
SELECT    → kaunse columns?
```

### MongoDB mapping

```javascript
// ASC
.sort({ salary: 1 })

// DESC
.sort({ salary: -1 })
```

```text
SQL ASC   → Mongo 1
SQL DESC  → Mongo -1
```

---

## 7. LIMIT = Kitni Rows?

`LIMIT` result mein **kitni rows chahiye** ye control karta hai.

```sql
SELECT name, salary
FROM employees
LIMIT 3;
```

→ sirf 3 rows.

### 🔥 Important Combo: ORDER BY + LIMIT

Highest salary wale 3 employees:

```sql
SELECT name, salary
FROM employees
ORDER BY salary DESC
LIMIT 3;
```

Flow:

```text
employees
   ↓
ORDER BY salary DESC
   ↓
highest salary first
   ↓
LIMIT 3
   ↓
top 3 employees
```

⚠️ `LIMIT` khud ranking nahi karta. Ye current order mein se first N rows deta hai. Top N chahiye toh usually `ORDER BY + LIMIT` use karo.

### MongoDB mapping

```javascript
db.employees
  .find()
  .sort({ salary: -1 })
  .limit(3)
```

```text
SQL       → MongoDB
LIMIT 3   → .limit(3)
ASC       → 1
DESC      → -1
```

---

# SQL — Aggregate Functions

## Aggregate Functions

Multiple rows → ek calculation/result.

```text
COUNT() → kitne?
SUM()   → total?
AVG()   → average?
MAX()   → highest?
MIN()   → lowest?
```

### COUNT

```sql
SELECT COUNT(*)
FROM employees;
```

`COUNT(*)` → total rows.

```sql
SELECT COUNT(id)
FROM employees;
```

`COUNT(id)` → `id` ki non-NULL values count.

```text
COUNT(*)       → all rows
COUNT(column)  → non-NULL values in that column
```

### SUM

```sql
SELECT SUM(salary)
FROM employees;
```

→ total salary.

### AVG

```sql
SELECT AVG(salary)
FROM employees;
```

→ average salary.

### MAX

```sql
SELECT MAX(salary)
FROM employees;
```

→ highest salary.

### MIN

```sql
SELECT MIN(salary)
FROM employees;
```

→ lowest salary.

> `SUM/AVG/MAX/MIN` mainly numeric columns par meaningful hote hain (`salary`, `age`, etc.).

---

# SQL — GROUP BY & HAVING

## GROUP BY = Same values ko groups mein jama karo

Example:

```text
name   department   salary
Rahul  IT            50000
Aman   IT            70000
Priya  HR            60000
Rohit  HR            80000
Neha   Sales         40000
```

```sql
SELECT department, AVG(salary)
FROM employees
GROUP BY department;
```

Mental model:

```text
GROUP BY department
        ↓
IT group | HR group | Sales group
        ↓
AVG(salary) har group par
```

Result:

```text
department   avg_salary
IT           60000
HR           70000
Sales        40000
```

> **GROUP BY khud calculation nahi karta.** Ye sirf rows ko groups mein todta hai. `COUNT/SUM/AVG/MAX/MIN` group ke andar calculation karte hain.

### WHERE vs GROUP BY vs HAVING

```text
WHERE
→ individual ROWS filter

GROUP BY
→ rows ko GROUPS mein todta hai

HAVING
→ GROUPS filter karta hai
```

Example:

```sql
SELECT department, AVG(salary)
FROM employees
GROUP BY department
HAVING AVG(salary) > 60000;
```

→ pehle departments ke groups banao → average nikalo → sirf average > 60000 wale groups rakho.

---

# 🔥 SQL → MongoDB Aggregation Mental Mapping

MongoDB Aggregation ko **SQL ka pipeline version** samjho.

### Main Cheat Sheet

```text
SQL                         MongoDB Aggregation
------------------------------------------------------
FROM employees              → db.employees.aggregate([])
WHERE condition             → $match
GROUP BY department         → $group
COUNT(*)                    → $count / $sum: 1
SUM(salary)                 → $sum: "$salary"
AVG(salary)                 → $avg: "$salary"
MAX(salary)                 → $max: "$salary"
MIN(salary)                 → $min: "$salary"
HAVING condition            → $match AFTER $group
SELECT columns              → $project
ORDER BY salary ASC         → $sort: { salary: 1 }
ORDER BY salary DESC        → $sort: { salary: -1 }
LIMIT 5                     → $limit: 5
```

### 🔥 Killer Trick

```text
SQL WHERE       → Mongo $match
SQL GROUP BY    → Mongo $group
SQL HAVING      → Mongo $match AFTER $group
```

**Why two `$match` positions?**

```text
Before $group → individual documents filter
After $group  → groups filter
```

So:

```text
WHERE  → $match BEFORE $group
HAVING → $match AFTER $group
```

### Pipeline Order Intuition

Mongo aggregation step-by-step chalti hai:

```text
$match
  ↓
$group
  ↓
$match
  ↓
$sort
  ↓
$limit
```

Example SQL:

```sql
SELECT department, AVG(salary) AS avgSalary
FROM employees
WHERE city = 'Gurgaon'
GROUP BY department
HAVING AVG(salary) > 70000
ORDER BY avgSalary DESC
LIMIT 3;
```

MongoDB:

```javascript
db.employees.aggregate([
  {
    $match: {
      city: "Gurgaon"
    }
  },
  {
    $group: {
      _id: "$department",
      avgSalary: { $avg: "$salary" }
    }
  },
  {
    $match: {
      avgSalary: { $gt: 70000 }
    }
  },
  {
    $sort: {
      avgSalary: -1
    }
  },
  {
    $limit: 3
  }
])
```

### One-line memory

> **SQL mein question ko clauses mein tod; Mongo mein same thought ko pipeline stages mein tod.**

---

# 🔥 Mixed SQL Interview Practice

Assume:

```text
employees
--------------------------------
id | name | department | salary | city
```

## Q1 — Top 3 IT/HR employees

> Find the **top 3 highest-paid employees** who are from either **IT or HR** department and whose salary is **between 50,000 and 1,00,000**. Return `name`, `department`, `salary`.

### Correct query

```sql
SELECT name, department, salary
FROM employees
WHERE department IN ('IT', 'HR')
AND salary BETWEEN 50000 AND 100000
ORDER BY salary DESC
LIMIT 3;
```

### Your mistakes

```text
❌ IN(in,hr)            → IT/HR values + quotes
❌ 2 WHERE              → second condition uses AND
❌ ORDER BY ASC         → highest = DESC
❌ ORDER BY without column → ORDER BY salary DESC
```

---

## Q2 — Name contains `a`, not HR, top 5

> Find `name` and `salary` of employees whose name contains `a`, who are **NOT from HR**, have salary above 60,000, and return only the **5 highest-paid** employees.

### Correct query

```sql
SELECT name, salary
FROM employees
WHERE name LIKE '%a%'
AND department != 'HR'
AND salary > 60000
ORDER BY salary DESC
LIMIT 5;
```

### Your mistakes

```text
❌ name '%a%'          → name LIKE '%a%'
❌ missing FROM        → FROM employees
❌ IS NOT 'HR'         → != 'HR' for normal value comparison
❌ DESC written as des → DESC
```

---

## Q3 — Count IT employees above 70k

> How many employees are from the IT department and have salary greater than 70,000?

### Correct query

```sql
SELECT COUNT(*)
FROM employees
WHERE department = 'IT'
AND salary > 70000;
```

### Your mistake

```text
❌ COUNT(salary) outside SELECT
```

`COUNT()` is an aggregate function, so it goes in `SELECT`.

`COUNT(salary)` would also work if you specifically wanted to count non-NULL salaries; for counting matching employees, `COUNT(*)` is the clean default.

---

# Quick SQL + Mongo Mapping

```text
SQL                         MongoDB
------------------------------------------------
WHERE                       query filter object
AND                         multiple fields / $and
OR                          $or
IN                          $in
BETWEEN                     $gte + $lte
LIKE                        Regex
IS NULL                     field: null
IS NOT NULL                 $ne: null
NOT IN                      $nin
NOT LIKE                    $not + regex
ORDER BY ASC                .sort({ field: 1 })
ORDER BY DESC               .sort({ field: -1 })
LIMIT                       .limit()
GROUP BY                    $group
HAVING                      $match after $group
COUNT / SUM / AVG / MAX / MIN  → aggregation accumulators
```

### Core mental model

```text
WHERE      → rows filter karo
SELECT     → final result mein columns choose karo
ORDER BY   → rows arrange karo
LIMIT      → kitni rows chahiye
GROUP BY   → same values ko groups mein jama karo
HAVING     → groups filter karo

IN       → multiple exact values
BETWEEN  → range
LIKE     → text pattern
NULL     → missing/unknown value
NOT      → condition ka opposite

COUNT → kitne?
SUM   → total?
AVG   → average?
MAX   → highest?
MIN   → lowest?
```
