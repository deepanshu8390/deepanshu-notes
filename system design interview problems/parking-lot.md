# Parking Lot — Quick Revision

## Core flow
`Vehicle → ParkingSpotManager → suitable free Spot → Ticket`

Exit: `Ticket → FeeCalculator → Payment → Spot.vacate() → Manager.releaseSpot()`

## Main classes
- `ParkingLot` → floors ko manage karta hai.
- `ParkingFloor` → parking spots rakhta hai.
- `ParkingSpot` → type + occupied state; `occupy()/vacate()`.
- `Vehicle` → license number + vehicle type.
- `Ticket` → vehicle, spot, entry time; parking session.
- `ParkingSpotManager` → available spots efficiently maintain/assign/release karta hai.
- `PaymentStrategy` → Card/UPI/Cash jaise payment behavior.
- `FeeCalculator` → parking fee calculate karta hai; multiple algorithms hon to Strategy use kar sakte hain.

## Patterns
- **Strategy** → payment method / variable fee algorithm.
- **Observer** → optional; spot state change ke notifications ke liye, core allocation ke liye nahi.
- **Template Method** → yahan required nahi; sirf common algorithm skeleton + variable child steps ho to use karo.

## Important rules
- Available spots ko vehicle/spot type ke basis par maintain karo.
- `find + remove/assign` concurrency mein atomic hona chahiye, warna same spot 2 vehicles ko mil sakta hai.
- Suitable spot na mile → `NO_SPOT_AVAILABLE`.
- `ParkingSpot` apni state manage kare; `ParkingSpotManager` allocation/index manage kare.

## Mental model
**Spot = state | Manager = allocation | Ticket = session | Strategy = variable behavior**

## CAP/HLD
Normal Parking Lot LLD mein CAP discuss nahi karna. Large distributed parking-management system explicitly poocha jaye tab HLD topics (DB, APIs, scaling, consistency, concurrency) aayenge.
