# SQL + MongoDB Notes

## SQL

### SELECT + FROM

```sql
SELECT name, salary
FROM employees;
```

- `SELECT` → which columns to show
- `FROM` → which table to read from

### WHERE

```sql
SELECT name
FROM employees
WHERE city = 'Delhi';
```

- `WHERE` → filters **rows**
- Mental model: **har row mein specific column ki value check karo**
- `SELECT` chooses columns; `WHERE` chooses rows.

### AND / OR

```sql
SELECT name
FROM employees
WHERE department = 'IT'
AND city = 'Gurgaon';
```

- `AND` → both conditions must be true.

```sql
SELECT name
FROM employees
WHERE city = 'Delhi'
OR city = 'Gurgaon';
```

- `OR` → at least one condition must be true.

---

## MongoDB

### AND

Multiple fields in the same filter object mean **implicit AND**:

```javascript
{
  department: "IT",
  city: "Gurgaon"
}
```

Explicit form:

```javascript
{
  $and: [
    { department: "IT" },
    { city: "Gurgaon" }
  ]
}
```

### OR

```javascript
{
  $or: [
    { city: "Delhi" },
    { city: "Gurgaon" }
  ]
}
```

### Quick Mapping

| Concept | SQL | MongoDB |
|---|---|---|
| Filter rows/documents | `WHERE` | `find()` filter |
| AND | `WHERE a AND b` | `{ a: x, b: y }` |
| Explicit AND | `AND` | `$and` |
| OR | `OR` | `$or` |
