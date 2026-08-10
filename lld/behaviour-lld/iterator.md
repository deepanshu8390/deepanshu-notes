# Iterator Design Pattern

## Content

> **Core learning flow:** Problem Statement → My Design 1 → Problem in Design 1 → My Design 2 → Problem in Design 2 → Vector Modification Problem → Iterator Intuition → Iterator Solution → C++ → Java → Key Points

---

# 1. Problem Statement

Suppose we have a `Playlist` containing songs:

```text
[A, B, C, D]
```

We want to support different ways of playing the playlist:

- Normal order
- Shuffle order
- Favorites only
- Future modes like repeat/filter etc.

Important requirement we discovered while thinking about the design:

> **Player ko ek time par bas ek song chahiye. Player ko poora vector manipulate karna zaroori nahi hai.**

---

# 2. My Design — Design 1

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

# 3. My Design — Design 2 (Strategy-like)

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

# 4. Problem in Design 2 — Original Vector Modification

Ab ek important problem notice hui.

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

# 5. Copy Banana Ek Option Hai — But Better Question

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

# 6. The Turning Point — Player ko Actually Kya Chahiye?

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

# 7. Iterator ki Intuition

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

# 8. `hasNext()` and `next()`

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

# 9. Iterator ka Actual Benefit

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

# 10. Concrete Iterators

Ab different traversal behaviours ko different Iterator classes de sakte hain.

```text
Playlist
   |
   +── SimplePlaylistIterator
   +── ShuffledPlaylistIterator
   +── FavoritesPlaylistIterator
```

Important:

> **In iterators ka purpose original vector ko modify karna nahi hai. They decide what `next()` should return.**

For example, Shuffle Iterator internally order maintain kar sakta hai:

```text
Original playlist:
[A B C D]

Shuffle Iterator order:
[2 0 3 1]
```

Then:

```text
next() → C   (songs[2])
next() → A   (songs[0])
next() → D   (songs[3])
next() → B   (songs[1])
```

Original playlist remains:

```text
[A B C D]
```

---

# 11. Iterator Structure — C++

## Iterator Interface

```cpp
class PlaylistIterator {
public:
    virtual bool hasNext() = 0;
    virtual string next() = 0;
    virtual ~PlaylistIterator() {}
};
```

## Playlist

```cpp
class Playlist {
private:
    vector<string> songs;

public:
    void addSong(string song) {
        songs.push_back(song);
    }

    string getSong(int index) {
        return songs[index];
    }

    int size() {
        return songs.size();
    }

    PlaylistIterator* createIterator();
};
```

## Simple Iterator

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

## Playlist creates the Iterator

```cpp
PlaylistIterator* Playlist::createIterator() {
    return new SimplePlaylistIterator(this);
}
```

## Client / Player

```cpp
Playlist playlist;

playlist.addSong("A");
playlist.addSong("B");
playlist.addSong("C");

PlaylistIterator* iterator = playlist.createIterator();

Player player;

while (iterator->hasNext()) {
    player.play(iterator->next());
}

delete iterator;
```

Flow:

```text
Client
  ↓
"next song?"
  ↓
Iterator.hasNext()
  ↓
Iterator.next()
  ↓
one song
  ↓
Player.play(song)
```

---

# 12. Same Idea — Java

## Iterator Interface

```java
public interface PlaylistIterator {
    boolean hasNext();
    String next();
}
```

## Playlist

```java
import java.util.ArrayList;

public class Playlist {
    private ArrayList<String> songs = new ArrayList<>();

    public void addSong(String song) {
        songs.add(song);
    }

    public String getSong(int index) {
        return songs.get(index);
    }

    public int size() {
        return songs.size();
    }

    public PlaylistIterator createIterator() {
        return new SimplePlaylistIterator(this);
    }
}
```

## Simple Iterator

```java
public class SimplePlaylistIterator implements PlaylistIterator {
    private Playlist playlist;
    private int index = 0;

    public SimplePlaylistIterator(Playlist playlist) {
        this.playlist = playlist;
    }

    @Override
    public boolean hasNext() {
        return index < playlist.size();
    }

    @Override
    public String next() {
        return playlist.getSong(index++);
    }
}
```

## Player

```java
public class Player {
    public void play(String song) {
        System.out.println("Playing: " + song);
    }
}
```

## Client

```java
public class Main {
    public static void main(String[] args) {

        Playlist playlist = new Playlist();

        playlist.addSong("A");
        playlist.addSong("B");
        playlist.addSong("C");

        PlaylistIterator iterator = playlist.createIterator();

        Player player = new Player();

        while (iterator.hasNext()) {
            player.play(iterator.next());
        }
    }
}
```

---

# 13. Strategy vs Iterator — Final Understanding

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
Playlist
   ↓
Iterator
   ↓
next()
   ↓
one song
   ↓
Player
```

The article's `ShufflePlaylistIterator` initially Strategy jaisa feel hona valid tha. Difference ko **name se nahi, responsibility se** samjho.

---

# 14. Key Points

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

# 15. One-Line Intuition

> **Player bole: "mujhe next song do." Iterator bole: "ye lo next song." Playlist bas apna data rakhe.**

### Aur sabse important learning

> **Pattern ko force mat karo. Pehle problem feel karo, phir responsibility identify karo, phir pattern derive karo.**
