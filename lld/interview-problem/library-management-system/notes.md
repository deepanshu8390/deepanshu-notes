# Library Management System

## Core Design Decisions

### Reservation is at Book level, not BookCopy level

> Because a user reserves a book/title, not a specific physical copy.

A reservation is made for the logical book/title. Physical copies are interchangeable for the reservation.

### Book vs BookCopy

- **Book** = logical book/title (id, title, author, type)
- **BookCopy** = physical copy of a book (copyId, book reference, status)

Example:

```text
Clean Code
├── Copy C1 → ISSUED
├── Copy C2 → AVAILABLE
└── Copy C3 → AVAILABLE
```

### Manager vs BookCopy

- `LibraryManager` manages/finds the physical copies.
- `LibraryManager.findAvailableCopy(book)` finds an available copy.
- `BookCopy` owns its own availability/state transition.
- Manager should not directly manipulate `copy.status`; delegate to `copy.issueTo(user)`.

Mental model:

```text
Manager → finds the object
Object  → performs its own state transition
```

## Core Entities

### Library

Coordinates library operations and holds a reference to `LibraryManager`.

### LibraryManager

Manages the inventory of `BookCopy` objects and finds an available copy for a given `Book`.

### Book

Represents the logical book/title.

Possible fields:

- id
- title
- author
- type
- reservation queue

### BookCopy

Represents a physical copy of a `Book`.

Possible fields:

- copyId
- book reference
- status

Possible operations:

- `isAvailable()`
- `issueTo(user)`
- `returnBook()`

### User

Represents a library user who can borrow, return, or reserve books.

### Loan

Represents a borrowing transaction.

Possible fields:

- user
- bookCopy
- issueDate
- dueDate
- returnDate

### Reservation

Represents a user's reservation for a logical `Book`.

Possible fields:

- user
- book
- reservedAt
- status

### FineCalculator

Calculates fines from a `Loan` when the book is returned late.

## Borrow Flow

```text
User
  ↓ borrow(book)
Library
  ↓
LibraryManager
  ↓ findAvailableCopy(book)
BookCopy
  ↓ issueTo(user)
AVAILABLE → ISSUED
```

The user delegates the operation instead of knowing how the library inventory is organized.

## Return Flow

```text
User
  ↓ returnBook(copy)
Library
  ↓
BookCopy.returnBook()
  ↓
ISSUED → AVAILABLE
```

A physical `BookCopy` is returned, not the abstract `Book`.

## Reservation Flow

If no physical copy is available:

```text
User
  ↓ reserve(book)
Library
  ↓
Book.reservationQueue.add(reservation)
```

When a copy becomes available, the reservation queue can be used to notify/serve the next waiting user.

### Reservation Queue + Notification Mental Model

Suppose all copies of `Clean Code` are issued:

```text
Book: Clean Code
reservationQueue:
    Reservation(User A)
    Reservation(User B)
    Reservation(User C)
```

When one physical copy is returned:

```text
BookCopy.returnBook()
        ↓
status = AVAILABLE
        ↓
Book checks reservationQueue
        ↓
get first Reservation
        ↓
User A
        ↓
notify(User A)
```

Only the **first waiting user** is notified because reservations are FIFO. We do not notify every user at once.

Important distinction:

> `BookCopy` owns physical availability, `Book` owns the reservation queue, and `Reservation` represents the waiting request.

### Subject / Observer Clarification

If we deliberately apply Observer Pattern:

```text
Book = Subject
        ↓
   subscribers/observers
        ↓
Reservation/User
```

The important point is that simply doing `book.hasReservation()` is **not by itself Observer Pattern**. That is just `Book` checking its reservation queue.

For the basic Library Management problem, Observer Pattern is optional. A simple implementation can directly get the first reservation and notify its user.

If the interviewer specifically asks for decoupled notifications, then Observer Pattern can be introduced:

```text
BookCopy becomes AVAILABLE
        ↓
Book
        ↓
notifyObservers()
        ↓
first waiting subscriber
        ↓
notify User
```

## Observer Pattern

Do not introduce Observer just for borrowing/returning state changes.

It becomes useful for the reservation requirement when the system needs to notify waiting users about availability.

Core mental model:

> **State changes → Subject notifies subscribers/observers.**

For this problem:

```text
BookCopy → physical state changes
Book     → owns title-level reservation state
User     → waiting subscriber who needs the notification
```

## Basic Code Skeleton

The following is intentionally a simple interview-level skeleton. The goal is to show **responsibility boundaries**, not production-level infrastructure.

```python
from enum import Enum
from collections import deque


class CopyStatus(Enum):
    AVAILABLE = "AVAILABLE"
    ISSUED = "ISSUED"


class Reservation:
    def __init__(self, user, book):
        self.user = user
        self.book = book


class User:
    def __init__(self, user_id, name):
        self.user_id = user_id
        self.name = name

    def notify(self, book):
        print(f"{book.title} is now available for you")

    def borrow(self, book, library):
        return library.borrow(self, book)

    def reserve(self, book, library):
        return library.reserve(self, book)


class Book:
    def __init__(self, book_id, title):
        self.book_id = book_id
        self.title = title
        self.reservation_queue = deque()

    def add_reservation(self, reservation):
        self.reservation_queue.append(reservation)

    def get_next_reservation(self):
        if not self.reservation_queue:
            return None
        return self.reservation_queue.popleft()


class BookCopy:
    def __init__(self, copy_id, book):
        self.copy_id = copy_id
        self.book = book
        self.status = CopyStatus.AVAILABLE
        self.borrowed_by = None

    def is_available(self):
        return self.status == CopyStatus.AVAILABLE

    def issue_to(self, user):
        if not self.is_available():
            return False

        self.status = CopyStatus.ISSUED
        self.borrowed_by = user
        return True

    def return_book(self):
        self.status = CopyStatus.AVAILABLE
        self.borrowed_by = None


class LibraryManager:
    def __init__(self):
        self.copies = {}

    def add_copy(self, copy):
        self.copies.setdefault(copy.book.book_id, []).append(copy)

    def find_available_copy(self, book):
        for copy in self.copies.get(book.book_id, []):
            if copy.is_available():
                return copy
        return None


class Library:
    def __init__(self, manager):
        self.manager = manager

    def borrow(self, user, book):
        copy = self.manager.find_available_copy(book)

        if copy is None:
            self.reserve(user, book)
            return None

        copy.issue_to(user)
        return copy

    def reserve(self, user, book):
        reservation = Reservation(user, book)
        book.add_reservation(reservation)

    def return_book(self, copy):
        copy.return_book()

        reservation = copy.book.get_next_reservation()
        if reservation:
            reservation.user.notify(copy.book)
```

### Code Responsibility Check

```text
Library
  → coordinates the use case

LibraryManager
  → finds an available BookCopy

BookCopy
  → changes AVAILABLE ↔ ISSUED

Book
  → owns reservation queue

Reservation
  → represents one waiting request

User
  → receives notification / makes requests
```

The most important design rule demonstrated by the code:

> **Manager finds the object; the object performs its own state transition.**

## Responsibility Summary

| Class | Responsibility |
|---|---|
| `User` | Requests borrow/return/reservation; receives notification |
| `Library` | Coordinates library operations |
| `LibraryManager` | Finds/manages available physical copies |
| `Book` | Represents logical title and reservation queue |
| `BookCopy` | Maintains physical-copy state |
| `Loan` | Tracks a borrowing transaction |
| `Reservation` | Tracks a waiting request for a book |
| `FineCalculator` | Calculates late-return fine |

## Important Mental Model

> **Loan tracks a physical copy. Reservation tracks a book/title.**

```text
Book        = logical title
BookCopy    = physical copy
Loan        = borrowing transaction
Reservation = waiting for a book
```

### Final Reservation Mental Model

> **Book is the title-level subject, the user is the actual party that needs the notification, and the reservation object records the waiting request. When a copy becomes available, notify the first waiting user.**
