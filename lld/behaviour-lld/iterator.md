# Iterator Design Pattern

## Content

> **Core learning flow:** Problem Statement → My Design 1 → Problem in Design 1 → My Design 2 → Problem in Design 2 → Vector Modification Problem → Iterator Intuition → Iterator Solution → C++ → Java → Key Points

---

> ## Java Quick Note — `.contains()`
>
> `contains()` ka simple meaning hai: **"kya ye value andar present hai?"**
>
> ### String
>
> ```java
> String song = "Song 2 Fav";
>
> song.contains("Fav"); // true
> ```
>
> Yahan String ke andar `"Fav"` text search ho raha hai.
>
> ### ArrayList
>
> ```java
> ArrayList<String> songs = new ArrayList<>();
>
> songs.add("A");
> songs.add("B");
> songs.add("C");
>
> if (songs.contains("A")) {
>     System.out.println("A is present");
> }
> ```
>
> Yahan check ho raha hai ki **`"A"` list ke elements mein present hai ya nahi**.
>
> ### Time Complexity
>
> ```text
> ArrayList.contains(value) → O(n) worst case
> ```
>
> Reason: ArrayList mein value search ke liye elements ko one-by-one compare karna pad sakta hai.
>
> Best case mein first element mil jaye toh `O(1)`.
>
> Useful comparison:
>
> ```text
> ArrayList.contains(x) → O(n)
> HashSet.contains(x)   → O(1) average
> ```
>
> **Same `.contains()` naam hone ke baad bhi underlying data structure complexity change kar deta hai.**
>
> ---

# 1. Clean Iterator Problem — Employee Collection

Playlist example ko side mein rakhkar ek cleaner problem dekho.

Imagine karo company ke employees hain:

```text
[Ram, Shyam, Amit, Rahul, Neha]
```

Client ko employees ko **one-by-one process** karna hai.

Lekin company internally employees ko kaise store karti hai, ye client ka concern nahi hona chahiye.

Aaj collection internally:

```text
ArrayList / vector
```

ho sakti hai.

Kal implementation change hokar:

```text
LinkedList
```

ho sakti hai.

Ya future mein koi custom collection/data structure ho sakta hai.

## Actual Problem

Without abstraction, client ko collection ke internal structure ke according traversal likhna padega.

Array/vector case:

```cpp
for (int i = 0; i < employees.size(); i++) {
    process(employees[i]);
}
```

Linked list case mein traversal alag hoga:

```text
head
  ↓
current
  ↓
current->next
  ↓
null
```

So:

> **Collection ka internal data structure change hua → client ka traversal code bhi change ho sakta hai.**

Aur client ka actual kaam sirf itna hai:

```text
employee lo
    ↓
process employee
    ↓
next employee
    ↓
process employee
```

Client ko ye decide nahi karna chahiye ki **next employee kaise find hua**.

### Core problem statement

> **How can we allow a client to sequentially access elements of a collection without exposing the collection's internal data structure or traversal logic?**

Baby version:

> **Client bas bole: "next employee de." Usko nahi pata hona chahiye ki employee vector, linked list, tree ya kisi custom structure se aa raha hai.**

---

# 2. Iterator Intuition — Solution Naturally Derive Karo

Ab hum directly "Iterator Pattern" nahi bolenge. Problem se derive karte hain.

Client ko poora collection nahi chahiye.

Client ko chahiye:

```text
next employee
next employee
next employee
```

So ek separate object bana do jo collection ke saath **current traversal position/state** rakhe.

```text
Employees
[Ram Shyam Amit Rahul]
   ↑
 current position
```

Client bole:

```text
"next employee?"
```

Iterator bole:

```text
"Ram lo"
```

Phir position aage:

```text
[Ram Shyam Amit Rahul]
       ↑
```

Next:

```text
"Shyam lo"
```

### Main realization

> **Collection ko modify nahi karna. Collection ko copy bhi zaroori nahi karna. Bas traversal state ko separate object mein rakhna hai.**

So:

```text
Collection = WHAT data exists
Iterator   = WHAT comes next
Client     = WHAT to do with the element
```

---

# 3. Iterator Interface

Iterator ko bas standard interface chahiye:

```cpp
class Iterator {
public:
    virtual bool hasNext() = 0;
    virtual string next() = 0;
    virtual ~Iterator() {}
};
```

### `hasNext()`

> "Kya aur employee bacha hai?"

### `next()`

> "Ek next employee do aur position aage badhao."

Important:

> **`next()` khud poora loop nahi karta. Sirf ONE element return karta hai.**

---

# 4. C++ — Complete Employee Iterator

```cpp
#include <iostream>
#include <vector>
#include <string>
using namespace std;

class Iterator {
public:
    virtual bool hasNext() = 0;
    virtual string next() = 0;
    virtual ~Iterator() {}
};

class EmployeeCollection {
private:
    vector<string> employees;

public:
    void addEmployee(string employee) {
        employees.push_back(employee);
    }

    string getEmployee(int index) {
        return employees[index];
    }

    int size() {
        return employees.size();
    }

    Iterator* createIterator();
};

class EmployeeIterator : public Iterator {
private:
    EmployeeCollection* collection;
    int index;

public:
    EmployeeIterator(EmployeeCollection* collection) {
        this->collection = collection;
        index = 0;
    }

    bool hasNext() override {
        return index < collection->size();
    }

    string next() override {
        return collection->getEmployee(index++);
    }
};

Iterator* EmployeeCollection::createIterator() {
    return new EmployeeIterator(this);
}

void process(string employee) {
    cout << "Processing: " << employee << endl;
}

int main() {
    EmployeeCollection employees;

    employees.addEmployee("Ram");
    employees.addEmployee("Shyam");
    employees.addEmployee("Amit");
    employees.addEmployee("Rahul");

    Iterator* iterator = employees.createIterator();

    while (iterator->hasNext()) {
        process(iterator->next());
    }

    delete iterator;
    return 0;
}
```

### C++ Client ko kya pata hai?

Sirf:

```cpp
while (iterator->hasNext()) {
    process(iterator->next());
}
```

Client ko nahi pata:

```text
vector hai?
index hai?
linked list hai?
pointer hai?
next element kaise mil raha hai?
```

---

# 5. Java — Complete Employee Iterator

## Iterator Interface

```java
public interface EmployeeIterator {
    boolean hasNext();
    String next();
}
```

## Employee Collection

```java
import java.util.ArrayList;

public class EmployeeCollection {

    private ArrayList<String> employees = new ArrayList<>();

    public void addEmployee(String employee) {
        employees.add(employee);
    }

    public String getEmployee(int index) {
        return employees.get(index);
    }

    public int size() {
        return employees.size();
    }

    public EmployeeIterator createIterator() {
        return new SimpleEmployeeIterator(this);
    }
}
```

## Concrete Iterator

```java
public class SimpleEmployeeIterator implements EmployeeIterator {

    private EmployeeCollection collection;
    private int index = 0;

    public SimpleEmployeeIterator(EmployeeCollection collection) {
        this.collection = collection;
    }

    @Override
    public boolean hasNext() {
        return index < collection.size();
    }

    @Override
    public String next() {
        return collection.getEmployee(index++);
    }
}
```

## Client

```java
public class Main {

    public static void main(String[] args) {

        EmployeeCollection employees = new EmployeeCollection();

        employees.addEmployee("Ram");
        employees.addEmployee("Shyam");
        employees.addEmployee("Amit");
        employees.addEmployee("Rahul");

        EmployeeIterator iterator = employees.createIterator();

        while (iterator.hasNext()) {
            String employee = iterator.next();
            System.out.println("Processing: " + employee);
        }
    }
}
```

### Java flow

```text
EmployeeCollection
        ↓
createIterator()
        ↓
EmployeeIterator
        ↓
hasNext()
        ↓
next()
        ↓
one employee
        ↓
client processes it
```

---

# 6. Why This Is Better Than Direct Traversal

Without Iterator:

```cpp
for (int i = 0; i < employees.size(); i++) {
    process(employees[i]);
}
```

Client knows the collection is index-based.

With Iterator:

```cpp
while (iterator->hasNext()) {
    process(iterator->next());
}
```

Client only knows the traversal contract.

If internal implementation changes from:

```text
vector
```

to:

```text
linked list
```

then we can change the concrete iterator implementation while keeping the client logic the same.

---

# 7. Original Playlist Example — My Design 1

Suppose we have a `Playlist` containing songs:

```text
[A, B, C, D]
```

We want to support different ways of playing the playlist:

- Normal order
- Shuffle order
- Favorites only
- Future modes like repeat/filter etc.

Initial thought:

```text
Playlist
   ↓
stores songs

Play (abstract)
   ↓
   ├── NormalPlay
   ├── ShufflePlay
   └── FavoritePlay
```

Har child apna logic rakhe:

- `NormalPlay` → normal order mein play
- `ShufflePlay` → pehle shuffle, phir play
- `FavoritePlay` → favorites select, phir play

## C++

```cpp
class Play {
public:
    virtual void play(vector<string>& songs) = 0;
};

class ShufflePlay : public Play {
public:
    void play(vector<string>& songs) override {
        // shuffle songs

        for (string song : songs) {
            cout << "Playing: " << song << endl;
        }
    }
};

class FavoritePlay : public Play {
public:
    void play(vector<string>& songs) override {
        // filter favorites

        for (string song : songs) {
            // play favorite
        }
    }
};
```

## Problem

Har child class ko apna `play()` loop likhna padega.

```text
ShufflePlay
    shuffle logic
    + for loop

FavoritePlay
    filter logic
    + for loop

NormalPlay
    + for loop
```

Yaani:

> **Actual changing logic alag hai, lekin playing loop same hai.**

Isse duplicate code aa sakta hai.

---

# 8. My Design 2 — Strategy-like

Phir better idea aaya:

```text
Playlist
   ↓
stores vector
   ↓
Strategy
   ├── NormalStrategy
   ├── ShuffleStrategy
   └── FavoriteStrategy
   ↓
Player
   ↓
common for loop
```

Yahaan Strategy ka kaam hai playlist ko prepare/rearrange/filter karna.

Player ka kaam sirf play karna hai.

## C++

```cpp
class Playlist {
private:
    vector<string> songs;

public:
    void addSong(string song) {
        songs.push_back(song);
    }

    vector<string>& getSongs() {
        return songs;
    }
};
```

Strategy interface:

```cpp
class PlaylistStrategy {
public:
    virtual void arrange(vector<string>& songs) = 0;
    virtual ~PlaylistStrategy() {}
};
```

Shuffle strategy:

```cpp
class ShuffleStrategy : public PlaylistStrategy {
public:
    void arrange(vector<string>& songs) override {
        // shuffle songs
    }
};
```

Favorite strategy:

```cpp
class FavoriteStrategy : public PlaylistStrategy {
public:
    void arrange(vector<string>& songs) override {
        // filter favorite songs
    }
};
```

Player:

```cpp
class Player {
public:
    void play(vector<string>& songs) {
        for (string song : songs) {
            cout << "Playing: " << song << endl;
        }
    }
};
```

Client:

```cpp
Playlist playlist;

playlist.addSong("A");
playlist.addSong("B Fav");
playlist.addSong("C");
playlist.addSong("D Fav");

ShuffleStrategy strategy;
strategy.arrange(playlist.getSongs());

Player player;
player.play(playlist.getSongs());
```

This is a **valid Strategy-style design**.

---

# 9. Problem in Design 2 — Original Vector Modification

Suppose original playlist hai:

```text
[A B C D]
```

Shuffle strategy vector ko modify karti hai:

```text
[A B C D]
    ↓ shuffle
[C A D B]
```

Ab **original playlist hi change ho gayi**.

Similarly favorites:

```text
[A B-Fav C D-Fav]
       ↓ filter
[B-Fav D-Fav]
```

Original data lose/change ho sakta hai.

Agar ek hi playlist ko baad mein normal order mein bhi play karna hai, problem aa sakti hai.

---

# 10. Copy Banana Ek Option Hai — But Better Question

Ek thought:

> "Original vector ko touch mat karo, copy bana lo."

Example:

```cpp
vector<string> copy = songs;
```

Then:

```text
Original:
[A B C D]

Copy:
[C A D B]
```

Original safe hai.

But ab har traversal behaviour ko apni copy manage karni pad sakti hai. Large collection mein unnecessary copying bhi ho sakti hai.

So better question:

> **"Mujhe vector ko modify/copy karna hi kyun hai?"**

---

# 11. The Turning Point — Player ko Actually Kya Chahiye?

Player ko ye nahi chahiye:

```text
poora vector
```

Player ko actually chahiye:

```text
next song
next song
next song
...
```

So problem ko reframe karo:

> **"Mujhe collection ko rearrange nahi karna. Mujhe bas ek object chahiye jo jab bhi poochun, next song de."**

Yahin se **Iterator** naturally emerge hota hai.

---

# 12. Iterator ki Intuition

Iterator ka kaam collection ko modify karna nahi hai.

Iterator collection ke saath traversal ki **current position/state** rakhta hai.

Example:

```text
Playlist
[A B C D]
 ^
 index = 0
```

Client:

```text
"next song?"
```

Iterator:

```text
A do.
index → 1
```

Again:

```text
"next song?"
```

Iterator:

```text
B do.
index → 2
```

And so on.

So:

```text
Playlist = WHAT data exists
Iterator = WHAT comes next
Player   = HOW to play/process it
```

---

# 13. `hasNext()` and `next()`

Iterator ke basic interface mein do important methods hain.

### `hasNext()`

Sirf check:

> **"Kuch aur element bacha hai?"**

### `next()`

Sirf:

> **"Ek next element do aur traversal state aage badhao."**

Important:

> **`next()` khud poora loop nahi karta.**

Client repeatedly `next()` maangta hai:

```cpp
while (iterator->hasNext()) {
    player.play(iterator->next());
}
```

Yahaan loop client mein hai, but client ko traversal ki internal details nahi pata.

---

# 14. Iterator ka Actual Benefit

Without Iterator:

```cpp
for (int i = 0; i < songs.size(); i++) {
    player.play(songs[i]);
}
```

Client ko pata hai:

- collection vector hai
- index use ho raha hai
- `size()` use karna hai
- `songs[i]` se element lena hai

With Iterator:

```cpp
while (iterator->hasNext()) {
    player.play(iterator->next());
}
```

Client ko bas pata hai:

```text
hasNext()
next()
```

Traversal ka internal mechanism Iterator handle karta hai.

---

# 15. Concrete Iterators

Different traversal behaviours ko different Iterator classes de sakte hain.

```text
Collection
   |
   +── SimpleIterator
   +── ReverseIterator
   +── FilterIterator
```

Important:

> **In iterators ka purpose original collection ko modify karna nahi hai. They decide what `next()` should return.**

For example, a reverse iterator can maintain a position from the end:

```text
Original:
[A B C D]

Reverse Iterator:
next() → D
next() → C
next() → B
next() → A
```

Original collection remains unchanged.

---

# 16. Java `.contains()` — Small DSA Note

```java
ArrayList<String> songs = new ArrayList<>();

songs.add("A");
songs.add("B");
songs.add("C");

if (songs.contains("A")) {
    System.out.println("A is present");
}
```

`ArrayList.contains(value)` → **O(n) worst case**, because it may compare elements one-by-one.

```text
ArrayList.contains(x) → O(n)
HashSet.contains(x)   → O(1) average
```

Same `.contains()` method name, but underlying data structure changes the complexity.

---

# 17. Strategy vs Iterator — Final Understanding

### Strategy

Question:

> **"Algorithm/operation ko kaise perform karna hai?"**

Example:

```text
Playlist
   ↓
ShuffleStrategy / FavoriteStrategy
   ↓
prepare/rearrange/filter
   ↓
Player
```

### Iterator

Question:

> **"Collection se next element kaunsa dena hai?"**

```text
Collection
   ↓
Iterator
   ↓
next()
   ↓
one element
   ↓
Client
```

The playlist example initially Strategy jaisa feel hona valid tha. Difference ko **name se nahi, responsibility se** samjho.

---

# 18. Key Points

1. Iterator is a **behavioral design pattern**.
2. Iterator ka main purpose **loop hatana nahi** hai.
3. Iterator ka main purpose hai **traversal logic ko client se abstract karna**.
4. `hasNext()` → kuch bacha hai?
5. `next()` → ek next element do + traversal state advance karo.
6. `next()` khud poora loop nahi karta.
7. Client repeatedly `next()` request karta hai.
8. Collection original data ko own karti hai; Iterator traversal state own karta hai.
9. Different Iterators same collection ko different ways se traverse kar sakte hain without modifying the original collection.
10. Strategy aur Iterator ka overlap feel ho sakta hai, but Strategy interchangeable algorithm/behaviour ke liye hai, while Iterator standardized one-by-one traversal ke liye hai.

---

# 19. One-Line Intuition

> **Client bole: "mujhe next element do." Iterator bole: "ye lo next element." Collection bas apna data rakhe.**

### Sabse important learning

> **Pattern ko force mat karo. Pehle problem feel karo → changing responsibility identify karo → phir pattern derive karo.**
