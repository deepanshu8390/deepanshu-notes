# LLD Foundations

- **Constructor define ≠ constructor call**
  - `Parent(int x) {}` → define
  - `Parent(x)` → call

- **Child object creation:** parent constructor is always called first.
  - Parent has default constructor → C++ calls it automatically.
  - Parent needs arguments → child uses `: Parent(args)`.

- `: Parent(args)` is **C++ syntax, not a design-pattern rule**.

- Write `: Parent(args)` when the **parent owns data that must be initialized**.

- Example: Decorator → `BaseDecorator HAS-A Dish`, so `BaseDecorator(Dish*)` initializes that wrapped `Dish`.

- Same rule applies to **Command, Composite, or any inheritance-based design**; pattern doesn't matter.
