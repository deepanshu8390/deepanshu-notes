# LLD Foundations

## Parent Constructor

- **Constructor define ≠ constructor call**
  - `Parent(int x) {}` → define
  - `Parent(x)` → call

- **Child object creation:** parent constructor is always called first.
  - Parent has default constructor → C++ calls it automatically.
  - Parent needs arguments → child uses `: Parent(args)`.

```cpp
class Parent {
public:
    Parent(int x) {}
};

class Child : public Parent {
public:
    Child(int x) : Parent(x) {}
};
```

Flow: `new Child(10)` → `Parent(10)` → `Child(10)`

- `: Parent(args)` is **C++ syntax, not a design-pattern rule**.

- Write `: Parent(args)` when the **parent owns data that must be initialized**.

### Why Decorator had it

```cpp
class BaseDecorator : public Dish {
protected:
    Dish* dish;

public:
    BaseDecorator(Dish* dish) {
        this->dish = dish;
    }
};

class Cheese : public BaseDecorator {
public:
    Cheese(Dish* dish) : BaseDecorator(dish) {}
};
```

`BaseDecorator` owns `Dish*`, so its constructor initializes that wrapped `Dish`. `Cheese` simply passes the `Dish` to its parent.

- This is **not mandatory** for Decorator; it is a clean C++ way to initialize parent-owned data.
- Same rule applies to **Command, Composite, or any inheritance-based design**; pattern doesn't matter.
