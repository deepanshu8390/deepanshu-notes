# Parking Lot — LLD Interview ⭐⭐⭐⭐⭐

## Intuition
Parking lot ko 3 layers mein socho:

```text
ParkingLot
  └── Floors
       └── ParkingSpots
```

Vehicle aati hai → suitable **FREE spot** find → ticket → park.
Vehicle exit → ticket se spot identify → spot FREE.

## Main Classes
```text
ParkingLot
- floors: vector<ParkingFloor>

ParkingFloor
- spots: vector<ParkingSpot>

ParkingSpot
- id
- type
- occupied
+ occupy()
+ vacate()

Vehicle
- licenseNumber
- type

Ticket
- id
- vehicle
- spot
- entryTime
```

### Relationship
```text
ParkingLot 1 ─── * ParkingFloor
ParkingFloor 1 ─── * ParkingSpot
Ticket ─── Vehicle
Ticket ─── ParkingSpot
```

## Spot Allocation
Simple approach: floors → spots scan karke first suitable FREE spot.

Better: `ParkingSpotManager` available spots ko type ke according maintain kare:

```text
CAR   → [S2, S3, S7]
BIKE  → [S1, S5]
TRUCK → [S4]
```

```text
ParkingSpotManager
+ findAvailableSpot(type)
+ releaseSpot(spot)
```

**Mental model:** jis class ke paas available-spots ka index/collection hai, wahi usse maintain kare.

## Vehicle Design
Abhi sirf type alag hai, behavior alag nahi; isliye simple:

```text
enum VehicleType { BIKE, CAR, TRUCK }

class Vehicle {
    string licenseNumber;
    VehicleType type;
};
```

Separate `Bike/Car/Truck` subclasses tab lao jab genuinely different behavior ho.

## Patterns — Important
Observer/Mediator ko main solution mat samjho.

- **Observer:** spot state change hone par interested objects ko notify karna ho.
- **Mediator:** multiple objects ki complex communication centralize karni ho.
- **Template Method:** base class algorithm ka skeleton fix kare aur child steps vary kare.

Parking Lot ke core mein in patterns ki zaroorat nahi; simple responsibility-based design better hai.

## Core Flow
```text
enter(vehicle)
  ↓
findAvailableSpot(vehicle.type)
  ↓
spot.occupy()
  ↓
generate Ticket

exit(ticket)
  ↓
spot.vacate()
  ↓
releaseSpot(spot)
```

## Interview Line
> “ParkingLot floors ko own karta hai, Floor spots ko. Allocation ko efficiently handle karne ke liye available spots ko vehicle type ke basis par index/manager mein maintain kar sakte hain.”

## Quick Revision
- Has-a: Lot → Floors → Spots
- Vehicle → common data + `VehicleType`
- Ticket → vehicle + assigned spot
- Find = available spot manager/index
- Park = remove/free → occupied
- Exit = occupied → release/free
- Patterns only when requirement justifies them
