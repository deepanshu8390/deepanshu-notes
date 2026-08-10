# Iterator Design Pattern

## Core Intuition

Iterator ka main idea **loop hatana nahi hai**.

Iterator ka idea hai:

> **Client ko sirf ek-ek next element chahiye; collection ko traverse karne ka internal logic Iterator ke andar hide ho.**

Simple mental model:

```text
Playlist / Collection
        |
        | owns data
        v
     [A B C D]
        |
        v
    Iterator
        |
        +-- hasNext() -> kuch bacha hai?
        +-- next()    -> ek next element do
        |
        v
      Client / Player
```

### 3 responsibilities

```text
Playlist  = WHAT data exists
Iterator  = WHAT comes next
Player    = HOW to play/process it
```

---

## Pehle confusion: Iterator vs Strategy

Playlist example mein pehle laga ki `ShuffleIterator`, `FavoritesIterator` etc. actually Strategy jaise hain — aur ye observation valid hai.

Agar requirement sirf ye hai:

> playlist ko shuffle/filter/normal order mein arrange karo aur phir ek Player us vector ko play kare

toh **Strategy** natural solution ho sakta hai:

```text
Playlist
   |
   | stores vector
   v
Strategy
   |
   +-- NormalStrategy
   +-- ShuffleStrategy
   +-- FavoriteStrategy
   |
   v
Player
   |
   +-- common for-loop
```

Strategy ka question:

> **"Operation/algorithm ko kaise perform karna hai?"**

Iterator ka question:

> **"Collection se next element kaunsa dena hai?"**

---

## Iterator ki problem ko derive kaise karein?

Pattern ko directly mat lagao. Problem se derive karo.

Suppose playlist hai:

```text
[A B C D]
```

Different child classes agar original vector ko modify/rearrange karne lagen:

```text
Shuffle  -> [C A D B]
Favorite -> [B D]
```

toh original playlist ka data change ho raha hai. Ye undesirable ho sakta hai.

Copy banana ek option hai, but phir har traversal behaviour ko apna collection copy/manage karna pad sakta hai.

Phir better question:

> **"Mujhe vector ko modify karna hi kyun hai?"**

Player ko actually poora vector nahi chahiye. Player ko bas:

```text
next song
next song
next song
```

chahiye.

Yahin se Iterator naturally emerge hota hai.

---

## Iterator ka actual mechanism

Iterator collection ko modify nahi karta. Iterator traversal ki **current position/state** rakhta hai.

Example:

```text
[A B C D]
 ^
 index = 0
```

`next()`:

```text
return A
index -> 1
```

Again:

```text
return B
index -> 2
```

So Iterator ke paas usually kuch state hoti hai:

```text
collection/reference
current position / index / node pointer
```

### `hasNext()`

Sirf check:

> "Kuch aur element bacha hai?"

### `next()`

Sirf:

> "Ek next element do aur traversal state aage badhao."

**`next()` khud poora loop nahi karta.**

Client loop karta hai:

```cpp
while (iterator->hasNext()) {
    player.play(iterator->next());
}
```

Yahan loop ka purpose hai repeatedly `next()` maangna. Traversal ki actual details Iterator ke andar abstracted hain.

---

## Simple Iterator flow

```text
Client / Player
      |
      | hasNext()?
      v
   Iterator
      |
      | yes
      v
   next()
      |
      v
   one song
      |
      v
   Player.play(song)
```

Client ko ye nahi pata:

- vector hai ya linked list
- index use ho raha hai ya node pointer
- next element kaise find ho raha hai
- collection internally kaise stored hai

Client ko bas standard interface pata hai:

```text
hasNext()
next()
```

---

## Shuffle Iterator ka intuition

Original playlist ko modify karna zaroori nahi:

```text
Playlist:
[A B C D]
```

Shuffle Iterator internally traversal order maintain kar sakta hai:

```text
order = [2 0 3 1]
```

Then:

```text
next() -> C  (songs[2])
next() -> A  (songs[0])
next() -> D  (songs[3])
next() -> B  (songs[1])
```

Original playlist remains:

```text
[A B C D]
```

So important point:

> **Same collection + different Iterator = different way of traversing, without necessarily changing the collection itself.**

---

## C++ skeleton

```cpp
class PlaylistIterator {
public:
    virtual bool hasNext() = 0;
    virtual string next() = 0;
    virtual ~PlaylistIterator() {}
};
```

Concrete iterator:

```cpp
class SimplePlaylistIterator : public PlaylistIterator {
private:
    Playlist* playlist;
    int index = 0;

public:
    SimplePlaylistIterator(Playlist* playlist) {
        this->playlist = playlist;
    }

    bool hasNext() override {
        return index < playlist->size();
    }

    string next() override {
        return playlist->getSong(index++);
    }
};
```

Client:

```cpp
Playlist playlist;

playlist.addSong("A");
playlist.addSong("B");
playlist.addSong("C");

PlaylistIterator* iterator = playlist.createIterator();

while (iterator->hasNext()) {
    player.play(iterator->next());
}
```

---

## Key Interview Points

1. Iterator is a **behavioral design pattern**.
2. It separates **collection/data management** from **traversal logic**.
3. `hasNext()` checks availability; `next()` returns one element and advances state.
4. Iterator does **not** mean "no loop". The client can still have a loop; the traversal implementation is what is abstracted.
5. The collection need not be modified for every traversal behaviour.
6. Different concrete iterators can provide different traversal behaviour over the same collection.
7. Strategy and Iterator can look similar when different traversal algorithms are represented as different classes. The key distinction is: **Strategy = interchangeable algorithm/behaviour; Iterator = standardized one-by-one traversal interface.**

---

## Biggest Learning / Pitfall

Pattern ko justify karne ke liye fake problem mat invent karo.

Agar simple vector hai aur ek hi traversal hai:

```cpp
for (int i = 0; i < songs.size(); i++) {
    play(songs[i]);
}
```

then Iterator may be unnecessary abstraction.

Iterator ka value tab feel hota hai jab:

- collection internals hide karne hain,
- multiple collection implementations ho sakti hain,
- traversal state ko client se hide karna hai,
- ya same collection ko different ways se traverse karna hai without mutating the collection.

### One-line intuition

> **Player asks: "next song do." Iterator decides: "next song kaunsa hai." Playlist bas data own karti hai.**
