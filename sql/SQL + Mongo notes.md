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

## Quick Comparison

```text
SQL:
WHERE condition
AND condition

WHERE condition
OR condition

MongoDB:
{ field: value, field: value }   → implicit AND

{ $and: [...] }                  → explicit AND

{ $or: [...] }                   → OR
```
