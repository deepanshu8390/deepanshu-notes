# SQL + Mongo Notes

## SQL — Basic Filtering

### SELECT + FROM

```sql
SELECT name, salary
FROM employees;
```

- `SELECT` → kaunse columns/output fields chahiye
- `FROM` → kis table se

### WHERE = Row Filter

```sql
SELECT name
FROM employees
WHERE city = 'Delhi';
```

> `WHERE` = **kaunsi rows chahiye?**
>
> `SELECT` = **kaunse columns dikhane hain?**

---

## AND / OR

### AND

```sql
SELECT name
FROM employees
WHERE department = 'IT'
AND city = 'Gurgaon';
```

Dono conditions TRUE honi chahiye.

### OR

```sql
SELECT name
FROM employees
WHERE city = 'Delhi'
OR city = 'Gurgaon';
```

Koi ek TRUE ho → row select.

### MongoDB

Implicit AND:

```javascript
db.employees.find({
  department: "IT",
  city: "Gurgaon"
})
```

Explicit AND:

```javascript
{
  $and: [
    { department: "IT" },
    { city: "Gurgaon" }
  ]
}
```

OR:

```javascript
{
  $or: [
    { city: "Delhi" },
    { city: "Gurgaon" }
  ]
}
```

---

# SQL — WHERE Operators

## IN

```sql
SELECT name
FROM employees
WHERE department IN ('HR', 'IT');
```

`IN` = multiple exact values; short form of `OR`.

Mongo:

```javascript
db.employees.find({
  department: { $in: ["HR", "IT"] }
})
```

```text
SQL → IN
Mongo → $in
```

## BETWEEN

```sql
SELECT name, salary
FROM employees
WHERE salary BETWEEN 45000 AND 60000;
```

Both endpoints included.

Mongo:

```javascript
db.employees.find({
  salary: {
    $gte: 45000,
    $lte: 60000
  }
})
```

```text
SQL → BETWEEN
Mongo → $gte + $lte
```

## LIKE

```sql
SELECT name
FROM employees
WHERE name LIKE '%a%';
```

```text
'R%'  → R se start
'%n'  → n pe end
'%a%' → a contains
```

Mongo uses regex:

```text
/^R/      → R se start
/n$/      → n pe end
/a/       → a contains
/^R.*n$/  → R se start AND n pe end
```

```javascript
{ name: /^R/ }
{ name: /n$/ }
{ name: /a/ }
{ name: /^R.*n$/ }
```

```text
^  → start
$  → end
.* → zero or more characters
```

## NULL — DANGER ZONE 🚨

`NULL` ko `=` se check mat karo.

```sql
WHERE city IS NULL
WHERE city IS NOT NULL
```

Mongo:

```javascript
{ city: null }
{ city: { $ne: null } }
```

## NOT IN

```sql
WHERE department NOT IN ('HR', 'IT')
```

Mongo:

```javascript
{ department: { $nin: ["HR", "IT"] } }
```

## NOT LIKE

```sql
WHERE name NOT LIKE 'R%'
```

Mongo:

```javascript
{ name: { $not: /^R/ } }
```

---

# SQL — Sorting & Limiting

## ORDER BY

```sql
SELECT name, salary
FROM employees
ORDER BY salary DESC;
```

```text
ASC  → small → large
DESC → large → small
```

Mongo:

```javascript
.sort({ salary: 1 })   // ASC
.sort({ salary: -1 })  // DESC
```

## LIMIT

```sql
SELECT name, salary
FROM employees
LIMIT 3;
```

Mongo:

```javascript
.limit(3)
```

### ORDER BY + LIMIT

```sql
SELECT name, salary
FROM employees
ORDER BY salary DESC
LIMIT 3;
```

```text
ORDER BY DESC → highest first
LIMIT 3       → first 3
```

Mongo:

```javascript
db.employees
  .find()
  .sort({ salary: -1 })
  .limit(3)
```

---

# SQL — Aggregate Functions

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

`COUNT(id)` → `id` ki non-NULL values.

```text
COUNT(*)      → all rows
COUNT(column) → non-NULL values
```

### SUM / AVG / MAX / MIN

```sql
SELECT SUM(salary) FROM employees;
SELECT AVG(salary) FROM employees;
SELECT MAX(salary) FROM employees;
SELECT MIN(salary) FROM employees;
```

Mainly numeric columns par meaningful.

---

# SQL — GROUP BY & HAVING

## GROUP BY

> Same value wali rows ko ek group mein jama karo.

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

`GROUP BY` khud calculation nahi karta; `COUNT/SUM/AVG/MAX/MIN` group ke andar calculation karte hain.

## WHERE vs GROUP BY vs HAVING

```text
WHERE     → individual ROWS filter
GROUP BY  → rows ko GROUPS mein todta hai
HAVING    → GROUPS filter karta hai
```

Example:

```sql
SELECT department, AVG(salary)
FROM employees
GROUP BY department
HAVING AVG(salary) > 60000;
```

---

# 🔥 SQL → MongoDB Aggregation Mental Mapping

MongoDB Aggregation ko **SQL ka pipeline version** samjho.

### Main Cheat Sheet

```text
SQL                         MongoDB Aggregation
------------------------------------------------------
FROM employees              → collection + aggregate([])
WHERE condition             → $match
GROUP BY department         → $group
COUNT(*)                    → $count / $sum: 1
SUM(salary)                 → $sum: "$salary"
AVG(salary)                 → $avg: "$salary"
MAX(salary)                 → $max: "$salary"
MIN(salary)                 → $min: "$salary"
HAVING condition            → $match AFTER $group
SELECT columns              → $project
ORDER BY ASC                → $sort: { field: 1 }
ORDER BY DESC               → $sort: { field: -1 }
LIMIT 5                     → $limit: 5
```

### 🔥 Killer Trick

```text
SQL WHERE       → Mongo $match
SQL GROUP BY    → Mongo $group
SQL HAVING      → Mongo $match AFTER $group
```

```text
$match BEFORE $group → individual documents filter (WHERE)
$match AFTER $group  → groups filter (HAVING)
```

---

# ⭐ SELECT vs $project — Output Fields

This is about **what fields should be present in the final output**.

### SQL

```sql
SELECT name, salary
FROM employees;
```

→ only `name` and `salary` columns in output.

### MongoDB

Without `$project`, stages like `$match`, `$sort`, `$limit` normally keep the **complete document**.

```javascript
db.employees.aggregate([
  { $match: { department: "IT" } },
  { $sort: { salary: -1 } },
  { $limit: 3 }
])
```

→ 3 complete documents; all existing fields remain.

To choose output fields, use `$project`:

```javascript
{
  $project: {
    name: 1,
    salary: 1
  }
}
```

### `_id` bhi nahi chahiye?

Mongo `$project` mein `_id` default se included hota hai, so explicitly remove it:

```javascript
{
  $project: {
    _id: 0,
    name: 1,
    salary: 1
  }
}
```

```text
1 → include
0 → exclude
```

### Mental mapping

```text
SQL
SELECT name, salary
        ↓
Mongo
$project: {
  _id: 0,
  name: 1,
  salary: 1
}
```

> **SELECT / $project = final output mein kaunse fields chahiye?**

---

# Mongo Aggregation Structure

Aggregation pipeline = **array of stage objects**.

```javascript
db.employees.aggregate([
  { $match: { department: "IT" } },
  { $sort: { salary: -1 } },
  { $limit: 3 }
])
```

Mental structure:

```text
aggregate()
   ↓
ARRAY of objects
   ↓
each object = one pipeline stage
   ↓
stage key → stage-specific value/config
```

### ⭐ Pipeline Order Intuition + Stage Structure

Think about aggregation at **2 levels**:

**LEVEL 1 — Pipeline flow:**

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

**LEVEL 2 — Inside each stage:**

Each stage has its own structure. Do not memorize one universal inner syntax.

```text
$match
  ↓
field
  ↓
operator
  ↓
value
```

Example:

```javascript
{
  $match: {
    salary: {
      $gt: 60000
    }
  }
}
```

```text
$match
  ↓
salary
  ↓
$gt
  ↓
60000
```

Multiple fields in `$match` mean implicit AND:

```javascript
{
  $match: {
    city: "Gurgaon",
    department: "IT"
  }
}
```

Explicit `$and` / `$or` use an array of conditions:

```javascript
{
  $match: {
    $or: [
      { city: "Gurgaon" },
      { city: "Delhi" }
    ]
  }
}
```

### Common stage shapes

`$group` has a grouping key plus calculated/output fields:

```javascript
{
  $group: {
    _id: "$department",
    avgSalary: { $avg: "$salary" }
  }
}
```

```text
$group
 ├── _id → grouping key
 └── avgSalary → output field
                  ↓
                $avg
                  ↓
             "$salary"
```

`$sort`:

```javascript
{
  $sort: {
    salary: -1
  }
}
```

```text
$sort
  ↓
field
  ↓
1 / -1
```

`$limit`:

```javascript
{
  $limit: 3
}
```

```text
$limit
  ↓
number
```

### ⭐ Use the Structure, Don't Mug Up Syntax

Whenever you get a Mongo aggregation question:

```text
1. Identify the PIPELINE stages.
2. Put the stages in the correct ORDER.
3. For each stage, identify its INTERNAL STRUCTURE.
4. Fill field → operator → value / grouping key → accumulator / output.
```

Example thought process:

```text
Question:
IT employees from Gurgaon → highest salary → top 3 → only name/salary

Pipeline:
$match → $sort → $limit → $project

Inside stages:
$match  → department/city conditions
$sort   → salary: -1
$limit  → 3
$project → name: 1, salary: 1, _id: 0
```

> **Pipeline = stages ka sequence. Stage = apne purpose ke according specific structure.**

`avgSalary` koi reserved keyword nahi hai; developer khud naam deta hai.

`_id` in `$group` is special: it defines the **grouping key**.

```text
_id: "$department"
→ GROUP BY department
```

Example output:

```json
[
  { "_id": "IT", "avgSalary": 60000 },
  { "_id": "HR", "avgSalary": 70000 }
]
```

If you want a nicer output name, reshape after grouping with `$project`:

```javascript
{
  $project: {
    _id: 0,
    department: "$_id",
    avgSalary: 1
  }
}
```

---

# 🔥 Mixed SQL Interview Practice

Assume:

```text
employees
--------------------------------
id | name | department | salary | city
```

## Q1 — Top 3 IT/HR employees

> Find the top 3 highest-paid employees from IT or HR with salary between 50,000 and 1,00,000. Return name, department, salary.

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
❌ IN(in,hr) → IT/HR values + quotes
❌ 2 WHERE → second condition uses AND
❌ ASC → highest = DESC
❌ ORDER BY without column → ORDER BY salary DESC
```

---

## Q2 — Name contains `a`, not HR, top 5

> Find name and salary of employees whose name contains `a`, who are not from HR, have salary above 60,000, and return only the 5 highest-paid.

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
❌ name '%a%' → name LIKE '%a%'
❌ missing FROM → FROM employees
❌ IS NOT 'HR' → != 'HR' for normal value comparison
❌ des → DESC
```

---

## Q3 — Count IT employees above 70k

> How many employees are from IT and have salary greater than 70,000?

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

---

# Core Mental Model

```text
WHERE      → rows filter karo
SELECT     → output fields/columns choose karo
ORDER BY   → rows arrange karo
LIMIT      → kitni rows chahiye
GROUP BY   → same values ko groups mein jama karo
HAVING     → groups filter karo

IN       → multiple exact values
BETWEEN  → range
LIKE     → text pattern
NULL     → missing/unknown value
NOT      → opposite condition

COUNT → kitne?
SUM   → total?
AVG   → average?
MAX   → highest?
MIN   → lowest?
```

## SQL → Mongo one-line memory

> **Question ko pehle data-flow mein socho. Phir SQL clauses ya Mongo pipeline stages mein translate karo.**

```text
SQL WHERE      → Mongo $match
SQL GROUP BY   → Mongo $group
SQL HAVING     → Mongo $match after $group
SQL SELECT     → Mongo $project
SQL ORDER BY   → Mongo $sort
SQL LIMIT      → Mongo $limit
```
