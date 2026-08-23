# Decorator

- **Trigger:** `Naya type` vs `existing combination/add-on` → combination ho to Decorator consider.
- Add-ons ke combinations se **class explosion** hota hai.
- **Alternative:** `Order HAS-A vector<Addon>`; `Addon` abstract contract → `Cheese/Sauce/Fries` implement it.
- `calculatePrice()` → base price + `addon.getPrice()`; new addon ke liye existing calculation modify nahi hoti (**OCP**).
- **Why IS-A?** `Cheese(Burger)` ke baad Sauce ko Burger nahi, **Dish** chahiye. `Cheese` khud `Dish` hona chahiye, taaki `Sauce(Cheese(Burger))` chain ho sake.
- Decorator: **IS-A + HAS-A** → same contract follow karta hai + wrapped object rakhta hai.
- Key intuition: **Combination ko new class mat banao; wrap karke compose karo.**
