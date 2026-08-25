# Proxy Design Pattern

## Core intuition

**Proxy = Real Object ke saamne ek middleman / gatekeeper.**

```text
Client → Proxy → Real Object
```

Client directly Real Object se baat nahi karta. Proxy Real Object ke access ko control/manage karta hai.

### Real-life example

Office ke CEO se milne ke liye receptionist ke through jaana:

```text
You → Receptionist → CEO
```

Receptionist check kar sakta hai:
- appointment hai ya nahi
- permission hai ya nahi
- CEO available hai ya nahi

Receptionist = Proxy, CEO = Real Object.

---

## Proxy ka actual purpose

Proxy ka important point **relationship nahi, intent hai**.

Proxy generally same interface implement karta hai jo Real Object karta hai, aur Real Object ka reference/pointer hold karta hai.

```text
Image (interface)
├── RealImage
└── ImageProxy
       |
       └── HAS-A → RealImage
```

So:
- `ImageProxy IS-A Image`
- `ImageProxy HAS-A RealImage`

Proxy ka intent:

> Client ko Real Object tak direct access na dekar us access ko control/manage karna.

---

## Simple C++ example: Lazy Proxy

```cpp
class Image {
public:
    virtual void display() = 0;
};

class RealImage : public Image {
public:
    RealImage() {
        cout << "Loading huge image...\n";
    }

    void display() override {
        cout << "Displaying image\n";
    }
};

class ImageProxy : public Image {
    RealImage* realImage = nullptr;

public:
    void display() override {
        if (realImage == nullptr) {
            realImage = new RealImage();
        }

        realImage->display();
    }
};
```

Client:

```cpp
Image* image = new ImageProxy();

// RealImage abhi create nahi hui

image->display();

// Ab Proxy RealImage create karega
// aur display() ko RealImage ko delegate karega
```

Flow:

```text
new ImageProxy()
      ↓
RealImage not created

image->display()
      ↓
ImageProxy
      ↓
create RealImage
      ↓
RealImage::display()
```

### `RealImage*` kyun?

Proxy ko specifically actual `RealImage` ko create/control karna hai, isliye:

```cpp
RealImage* realImage;
```

`Image*` bhi technically possible hai:

```cpp
Image* realImage = new RealImage();
```

because `RealImage IS-A Image`. But `RealImage*` explicitly batata hai ki Proxy ka actual wrapped object `RealImage` hai.

---

## Delegation

**Delegation = "Ye kaam main khud nahi karunga, kisi aur object se karwaunga."**

Proxy mein delegation ho sakti hai:

```cpp
realImage->display();
```

Yahan Proxy actual display ka kaam `RealImage` ko delegate kar raha hai.

But:

> Delegation Proxy Pattern nahi hai. Delegation ek programming technique hai.

---

## Proxy ke common use cases

### 1. Lazy Proxy

Expensive object ko tab tak create nahi karna jab tak actual need na ho.

```text
Client → Proxy
           ↓
      only when needed
           ↓
       Real Object
```

### 2. Protection Proxy

Proxy permission check karta hai.

```text
Client
  ↓
Proxy
  ↓
checkPermission()
  ↓
Real Service
```

Example: sirf admin `deleteUser()` kar sakta hai.

### 3. Remote Proxy

Real object kisi remote server par ho sakta hai.

```text
Client → Proxy → Network → Remote Object
```

Client ko local method call jaisa feel hota hai, but Proxy internally network communication karta hai.

### 4. Caching Proxy

Proxy pehle cache check kar sakta hai. Cache mein data ho toh Real Object ko call hi nahi karega.

---

## Proxy vs Decorator

Dono structurally similar dikh sakte hain:

```text
Client → Middle Object → Real Object
```

Dono mein composition / HAS-A aur delegation mil sakte hain.

### Proxy

Intent:

> **Real Object ke access ko control/manage karna.**

Examples:
- permission
- lazy loading
- remote access
- caching

### Decorator

Intent:

> **Existing object ke behaviour mein additional behaviour add karna.**

Example:

```text
Real Object
    ↓
Logging Decorator
    ↓
Caching Decorator
```

So pattern ko sirf structure se identify nahi karna. **Intent matters.**

---

## Proxy vs Mediator

Proxy:

```text
Client → Proxy → Real Object
```

Focus: **ek object ke access ko control karna.**

Mediator:

```text
A ↔ Mediator ↔ B
       ↕
       C
```

Focus: **multiple objects ki communication ko centralize karna.**

Proxy ka beech mein hona usko Mediator nahi banata.

---

## Interview one-liner

> Proxy Pattern mein hum Real Object ke saamne ek substitute object rakhte hain jo client ke access ko control karta hai. Client Proxy se interact karta hai, aur Proxy decide karta hai ki Real Object ko kab aur kaise access karna hai.

## Key memory line

**Proxy = Real Object ke access ka gatekeeper.**

Structure common hai; **intent is what makes it Proxy.**
