# Problem 1 — Order State Management

## Problem

An ordering platform has an order whose status changes during its lifecycle:

`CREATED → PAID → CONFIRMED → PREPARING → OUT_FOR_DELIVERY → DELIVERED`

The system must control which transitions are allowed from the current state.

For example:

- `CREATED → PAID` is valid.
- `PAID → CONFIRMED` is valid.
- `DELIVERED → CANCELLED` is invalid.
- `PAID → DELIVERED` should not happen directly.

The client/dashboard receives the current order state and can use it to decide which actions to show. For example, it can show the **Cancel** button only when cancellation is allowed.

However, the client-side button is only a UI decision. The backend must validate the transition again because a client can be manipulated or can send an invalid request directly.

So the core problem is:

> **Based on the current order state, control which operations and state transitions are allowed.**

## Solution — State Pattern

Instead of putting all state-specific logic inside `Order` using large `if/else` or `switch` statements, represent each state as a separate class.

`Order` keeps a reference to its current state and delegates operations to that state.

Each concrete state decides what is allowed in that state.

For example:

- `CreatedState` can allow payment and cancellation.
- `PaidState` can allow confirmation and cancellation/refund.
- `DeliveredState` does not allow cancellation.

The state can also expose `canCancel()` so the application/client can know whether the Cancel action should be available.

## C++ Example

```cpp
#include <iostream>
using namespace std;

class Order;

class OrderState {
public:
    virtual bool canCancel() = 0;
    virtual void cancel(Order& order) = 0;
    virtual void pay(Order& order) = 0;
    virtual void confirm(Order& order) = 0;
    virtual ~OrderState() = default;
};

class Order {
private:
    OrderState* state;

public:
    Order(OrderState* initialState) : state(initialState) {}

    void setState(OrderState* newState) {
        state = newState;
    }

    bool canCancel() {
        return state->canCancel();
    }

    void cancel() {
        state->cancel(*this);
    }

    void pay() {
        state->pay(*this);
    }

    void confirm() {
        state->confirm(*this);
    }
};

class CreatedState : public OrderState {
public:
    bool canCancel() override {
        return true;
    }

    void cancel(Order& order) override {
        cout << "Order cancelled\n";
    }

    void pay(Order& order) override {
        cout << "Payment successful\n";
        // order.setState(new PaidState());
    }

    void confirm(Order& order) override {
        cout << "Cannot confirm. Payment pending\n";
    }
};

class PaidState : public OrderState {
public:
    bool canCancel() override {
        return true;
    }

    void cancel(Order& order) override {
        cout << "Order cancelled and refund initiated\n";
    }

    void pay(Order& order) override {
        cout << "Already paid\n";
    }

    void confirm(Order& order) override {
        cout << "Order confirmed\n";
        // order.setState(new ConfirmedState());
    }
};

class DeliveredState : public OrderState {
public:
    bool canCancel() override {
        return false;
    }

    void cancel(Order& order) override {
        cout << "Cannot cancel delivered order\n";
    }

    void pay(Order& order) override {
        cout << "Already paid\n";
    }

    void confirm(Order& order) override {
        cout << "Already delivered\n";
    }
};
```

## Key Idea

Without State Pattern:

```cpp
if (state == CREATED) { ... }
else if (state == PAID) { ... }
else if (state == CONFIRMED) { ... }
```

With State Pattern:

```text
Order
  ↓
Current State
  ↓
state.operation()
```

The current state owns the behavior for that state.

### Important

`canCancel()` is useful for deciding whether the UI should show the Cancel button, but the backend must still validate `cancel()` when the request actually arrives.
