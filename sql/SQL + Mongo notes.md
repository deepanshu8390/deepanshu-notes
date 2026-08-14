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
```

### Core mental model

```text
WHERE → rows filter karo
SELECT → final result mein columns choose karo

IN       → multiple exact values
BETWEEN  → range
LIKE     → text pattern
NULL     → missing/unknown value
NOT      → condition ka opposite
```
