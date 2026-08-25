# Problem 2 — ACID / Transaction Problem

## Problem

An ordering system has multiple operations involved in placing an order:

```text
Create Order
    ↓
Payment
    ↓
Inventory
    ↓
Confirm Order
```

If one operation fails after earlier operations have succeeded, the system can be left in an inconsistent state.

Example:

```text
Order created       ✅
Payment successful  ✅
Inventory update    ❌
```

Now the user has paid, but the order could not be completed.

## Solution — Single Database Transaction

If the operations are inside the same database transaction, use atomicity:

```text
BEGIN TRANSACTION

Create Order
Reduce Stock
Confirm Order

        ↓

All successful → COMMIT
Any failure     → ROLLBACK
```

The goal is: **either all database changes happen, or none of them happen.**

## C++ Simulation

```cpp
#include <iostream>
using namespace std;

class Database {
public:
    bool createOrder() {
        cout << "Order created\n";
        return true;
    }

    bool reduceStock() {
        cout << "Reducing stock...\n";
        return false; // simulate failure
    }

    void confirmOrder() {
        cout << "Order confirmed\n";
    }

    void rollback() {
        cout << "ROLLBACK: Undo all changes\n";
    }
};

class OrderService {
private:
    Database db;

public:
    void placeOrder() {
        cout << "BEGIN TRANSACTION\n";

        if (!db.createOrder()) {
            db.rollback();
            return;
        }

        if (!db.reduceStock()) {
            db.rollback();
            return;
        }

        db.confirmOrder();

        cout << "COMMIT TRANSACTION\n";
    }
};

int main() {
    OrderService service;
    service.placeOrder();
}
```

## Important: Real Ordering Systems

In a real ordering platform, Order, Payment, Inventory, Delivery etc. can be separate services with separate databases/systems.

A database transaction cannot simply rollback changes made in another independent service.

Example:

```text
Order Service       → Order DB
Payment Service     → Payment Gateway
Inventory Service   → Inventory DB
```

If:

```text
Order       ✅
Payment     ✅
Inventory   ❌
```

we cannot use one normal DB `ROLLBACK` to undo everything.

This leads to distributed transaction problems and concepts such as **Saga / compensating actions**, which can be studied separately.

## Facade vs ACID

**Facade:** hides a complex workflow behind a simple interface.

```text
Client
  ↓
Facade
  ↓
Payment + Inventory + Order + Delivery
```

**ACID / Transaction:** maintains consistency of transactional database operations.

```text
Operations
   ↓
All succeed → COMMIT
Any failure → ROLLBACK
```

Facade does **not** provide rollback or ACID guarantees.

## Key Takeaway

- ACID is not a design pattern.
- Atomicity means all-or-nothing for a transaction.
- `COMMIT` makes the transaction permanent.
- `ROLLBACK` undoes uncommitted database changes.
- A normal DB transaction does not automatically cover independent services.
- Distributed systems require additional approaches such as Saga, which will be studied separately.
