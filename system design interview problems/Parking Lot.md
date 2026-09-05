# Parking Lot — LLD Revision

## Core Idea
ParkingLot → Floors → ParkingSpots. `ParkingSpotManager` available spots ko vehicle/spot type ke basis par maintain karta hai, taaki har baar saare spots scan na karne pade.

## Main Classes
- **ParkingLot**: floors + SpotManager; entry/exit coordinate karta hai.
- **ParkingFloor**: floor ke spots rakhta hai.
- **ParkingSpot**: `id`, `spotType`, occupied state; `occupy()` / `vacate()`.
- **Vehicle**: `licenseNumber`, `vehicleType`.
- **Ticket**: vehicle + spot + entryTime; parking session represent karta hai.
- **ParkingSpotManager**: available spots assign/release karta hai.
- **PaymentService**: payment coordinate karta hai.

## Flow
**Entry:** Vehicle → SpotManager → suitable free Spot → `occupy()` → Ticket.

**Exit:** Ticket → fee calculate → Payment → `spot.vacate()` → SpotManager mein spot wapas.

## Patterns
- **Strategy**: payment methods (UPI/Card/Cash); fee calculation algorithms bhi variable hon to Strategy.
- **Observer**: optional; state-change notifications ke liye, core allocation ke liye required nahi.
- **Template Method**: is problem ke basic version mein required nahi.

## Important Design Rules
- Spot ki state Spot manage kare; available-spots collection SpotManager manage kare.
- Same spot ko 2 concurrent requests assign na kar paayen: allocation ka **find + remove + assign atomic/thread-safe** hona chahiye.
- Suitable spot na mile → `NO_SPOT_AVAILABLE`.
- Car ke liye Medium prefer, Large fallback (agar requirement allow kare).

## Mental Model
`Spot = state`  
`SpotManager = allocation`  
`Ticket = parking session`  
`PaymentStrategy = payment behavior`

## Interview Line
“Core design simple OOP hai; Strategy variable payment behavior ke liye use hota hai. Observer optional hai, aur unnecessary patterns force nahi karne chahiye.”
