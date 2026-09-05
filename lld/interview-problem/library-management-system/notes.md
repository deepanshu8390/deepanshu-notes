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
Book.reservationQueue.add(user)
```

When a copy becomes available, the reservation queue can be used to notify/serve the next waiting user.

## Observer Pattern

Do not introduce Observer just for borrowing/returning state changes.

It becomes useful for the reservation requirement:

```text
Book becomes available
        ↓
Reservation system
        ↓
Notify waiting users
```

## Responsibility Summary

| Class | Responsibility |
|---|---|
| `User` | Requests borrow/return/reservation |
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
Book       = logical title
BookCopy   = physical copy
Loan       = borrowing transaction
Reservation = waiting for a book
```
