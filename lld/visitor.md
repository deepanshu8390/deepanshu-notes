# Visitor Pattern

## Core Intuition

Visitor ka main use tab samjho jab:

- **Object/element types relatively fixed hain**.
- Lekin un objects par hone wale **operations continuously increase** ho rahe hain.
- Har operation ka behavior object ke concrete type ke according different ho sakta hai.

### Patient Example

Maan lo patient ke sirf 3 types hain:

- Child
- Young
- Old

Lekin operations badhte rahenge:

- X-Ray
- Blood Test
- MRI
- Surgery
- Insurance Check
- etc.

Problem ye hai ki har operation ke andar baar-baar ye nahi karna chahiye:

```text
if patient is Child
else if patient is Young
else if patient is Old
```

Visitor is type-checking ko structured way mein handle karta hai.

---

## Visitor ka Actual Flow

Patient side par `accept(visitor)` hota hai.

```text
Child.accept(XRayVisitor)
        ↓
XRayVisitor.visit(Child)
```

Similarly:

```text
Young.accept(XRayVisitor)
        ↓
XRayVisitor.visit(Young)
```

```text
Old.accept(XRayVisitor)
        ↓
XRayVisitor.visit(Old)
```

### Important Observation

Child ko kaise pata ki `visit(Child)` call karna hai?

`Child` ke `accept()` mein:

```text
accept(visitor) {
    visitor.visit(this);
}
```

Yahan `this` ka actual concrete type `Child` hai. Isliye visitor ka `visit(Child)` overload select hota hai.

---

## Visitor Structure

```text
Patient
 ├── Child
 ├── Young
 └── Old

Visitor
 ├── visit(Child)
 ├── visit(Young)
 └── visit(Old)
```

Different operations ko different visitors bana sakte hain:

```text
XRayVisitor
 ├── visit(Child)
 ├── visit(Young)
 └── visit(Old)

BloodTestVisitor
 ├── visit(Child)
 ├── visit(Young)
 └── visit(Old)
```

---

# Strategy vs Visitor

Ye distinction sabse important hai.

## Strategy

**Strategy = behavior/algorithm ko interchangeable banana.**

Question:

> "Kaunsa behavior/algorithm use karna hai?"

Example:

```text
Patient
   ↓ has-a
Operation
   ↓
XRay / BloodTest / MRI
```

Patient ke paas operation ka reference ho sakta hai aur operation dynamically change ho sakta hai.

So Strategy perfectly valid hai agar requirement simply ye hai:

> "Patient ko different operations/behaviors dene hain."

---

## Visitor

**Visitor = fixed types par naye operations add karna, especially jab har type ke liye operation ka behavior different ho.**

Question:

> "Is particular concrete type par ye operation kaise perform hoga?"

Example:

```text
Patient types fixed:
Child, Young, Old

Operations increasing:
XRay, BloodTest, MRI, Surgery...
```

Agar Strategy use karke har operation ko `perform(Patient)` diya, to andar phir ye aa sakta hai:

```text
if Child
else if Young
else if Old
```

Ye repeated type-checking Visitor solve karta hai.

---

## Very Important Correction

Ye rule **galat** hai:

```text
Fixed patients → Strategy
Dynamic patients → Visitor
```

Sahi way:

```text
Changing behavior/algorithm
        ↓
     Strategy

Fixed object types + increasing operations
        ↓
     Visitor
```

Isliye fixed number of patients hone par **Strategy use kar sakte ho**. Visitor automatically required nahi ho jata.

---

## Interview Intuition

Agar interviewer bole:

> "Patient ke 3 fixed types hain: Child, Young, Old. Lekin naye medical operations continuously add honge. Har operation patient type ke according different behave karega."

Thinking:

1. Strategy se operations abstract kar sakte hain.
2. Lekin har concrete Strategy ko `Patient` ka type check karna pad sakta hai.
3. Same `if/else` logic har operation mein repeat hoga.
4. Patient types stable hain, operations increase ho rahe hain.
5. Isliye Visitor better fit hai.

### One-line Memory Trick

> **Strategy = HOW will I do it?**
>
> **Visitor = WHAT new operation can I perform on these fixed types, without putting all that logic inside the types?**

---

## Key Takeaways

- Visitor ka focus **operations on a group of fixed types** hai.
- Strategy ka focus **interchangeable behavior/algorithm** hai.
- Fixed types hone ka matlab automatically Visitor nahi hai.
- Strategy aur Visitor ek hi system mein saath bhi use ho sakte hain.
- Visitor ka major benefit: repeated `if/else` / `instanceof` type checks ko avoid karna.
- `accept(visitor)` + `visitor.visit(this)` Visitor ka core flow hai.
- Agar naye **operations** frequently add ho rahe hain aur object types stable hain → Visitor strongly consider karo.
