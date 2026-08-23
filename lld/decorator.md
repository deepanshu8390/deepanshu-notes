# Decorator

- **Trigger:** `Naya type` vs `existing combination/add-on` → combination ho to Decorator consider.
- Add-ons ke combinations se **class explosion** hota hai.

## Alternative: Composition

- `Order HAS-A vector<Addon>`
- `Addon` = abstract contract → `Cheese/Sauce/Fries` implement it.
- `calculatePrice()` → base price + `addon.getPrice()`.
- New addon ke liye existing calculation modify nahi hoti → **OCP**.

## Decorator intuition

- Jab features ka **order/chain matter** kare, simple feature-list awkward ho sakti hai.
- `Dish` = common contract.
- `Cheese HAS-A Dish` and `Sauce HAS-A Dish`.
- **Why IS-A?** `Cheese(Burger)` ke baad Sauce ko specifically Burger nahi, **Dish** chahiye. Isliye `Cheese` khud `Dish` hona chahiye, taaki `Sauce(Cheese(Burger))` chain ho sake.
- Decorator = **IS-A + HAS-A** → same contract follow karta hai + wrapped object rakhta hai.

## Client flow

```cpp
Dish* dish = new Burger();
dish = new Cheese(dish);
dish = new Sauce(dish);
```

- Same variable is intentional: `dish` means **current/latest Dish**.
- First: `dish → Burger`
- After Cheese: `dish → Cheese → Burger`
- After Sauce: `dish → Sauce → Cheese → Burger`
- `dish = new Cheese(dish)` means: **new wrapper becomes current, old current object goes inside it**.
- Separate variables also work, but same variable keeps the client simple and makes the wrapping chain clear.

### Key intuition

**Combination ko new class mat banao; wrap karke compose karo.**
