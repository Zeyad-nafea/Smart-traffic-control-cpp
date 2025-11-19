# Data Structures and Algorithms - Project Phase 1.1

**Course:** Data Structures and Algorithms  
**Semester:** Spring 2025  
**Team Number:** 14  
**Team Email:** s-zeyad.nafea@zewailcity.edu.eg

## Team Members

| Member Name | ID | Email |
|------------|------------|-------|
| Zeyad Mohamed Fathy | 202400354 | s-zeyad.nafea@zewailcity.edu.eg |
| Omar Ahmed Fathy | 202401823 | s-omar.elgazar@zewailcity.edu.eg |
| Shaze Kazem Mahmoud | 202400948 | s-shaza.mahmoud@zewailcity.edu.eg |
| Ganna Allah Walid Helmy | 202401020 | s-ganna.wahba@zewailcity.edu.eg |

## Proposed Data Structures

### Vehicle Queue Management

#### 1. EV_Queue (Electric Vehicle Queue)
- **Data Structure:** Priority Queue
- **Class:** Vehicle
- **Reason:** EV vehicles have the highest priority
- **Operations:**
  - `AddVehicle()` - O(log n)
  - `UpdatePriority()` - O(log n)
  - `IsEmpty()` - O(1)

#### 2. PT_Queue (Public Transport Queue)
- **Data Structure:** Queue
- **Class:** Vehicle
- **Reason:** First-come-first-served with auto-promotion when WT ≥ AP
- **Operations:**
  - `Enqueue()` - O(1)
  - `Dequeue()` - O(1)
  - `PromoteIfWaitingTooLong()` - O(1)
  - `PrintQueue()` - O(n)

#### 3. NC_Queue (Normal Car Queue)
- **Data Structure:** Queue
- **Class:** Vehicle
- **Reason:** First-come-first-served with cancellation check when WT ≥ CancelT
- **Operations:**
  - `Enqueue()` - O(1)
  - `Dequeue()` - O(1)
  - `CheckCancellation()` - O(1)
  - `PrintQueue()` - O(n)

#### 4. FV_Queue (Fire Vehicle Queue)
- **Data Structure:** Queue
- **Class:** Vehicle
- **Reason:** Handles longer crossing times and locks lane while crossing
- **Operations:**
  - `Enqueue()` - O(1)
  - `Dequeue()` - O(1)
  - `LockLane()` - O(1)
  - `UnlockLane()` - O(1)

### Traffic Control Systems

#### 5. Intersections
- **Data Structure:** Array
- **Class:** Traffic Control
- **Reason:** Fixed number of intersections makes static array most efficient
- **Operations:**
  - `AccessIntersection(i)` - O(1)
  - `UpdateStatus()` - O(1)

#### 6. Lanes
- **Data Structure:** Array
- **Class:** Intersection
- **Reason:** Each intersection has a fixed set of lanes; accessed by lane index
- **Operations:**
  - `AccessLane(i)` - O(1)
  - `SwitchLane()` - O(1)
  - `DisplayLanes()` - O(n)

#### 7. Crossing Vehicles
- **Data Structure:** Array by Lane ID
- **Class:** Intersection
- **Reason:** Tracks vehicles currently crossing in each lane
- **Operations:**
  - `AddCrossingVehicle()` - O(1)
  - `RemoveCrossingVehicle()` - O(1)
  - `UpdateRemainingTime()` - O(1)

### Event Management

#### 8. Event Queue
- **Data Structure:** Priority Queue
- **Class:** EventScheduler
- **Reason:** Stores all events (Arrivals, Cancellations, Promotions, Accidents, Closures) ordered by event time
- **Operations:**
  - `insertEvent()` - O(log n)
  - `GetNextEvent()` - O(log n)

### Rerouting System

#### 9. Adjacency Map
- **Data Structure:** Array of Queues (Adjacency List)
- **Class:** ReroutingManager
- **Reason:** Represents connected intersections for rerouting; each queue stores neighboring intersections
- **Operations:**
  - `AddConnection(u,v)` - O(1)
  - `GetNeighbors(u)` - O(1)
  - `FindAlternatePath()` - O(V + E)

#### 10. Rerouting Queue
- **Data Structure:** Queue
- **Class:** ReroutingManager
- **Reason:** Temporarily stores intersections during rerouting when searching for alternative paths
- **Operations:**
  - `Enqueue()` - O(1)
  - `Dequeue()` - O(1)
  - `IsEmpty()` - O(1)

## Summary

This document outlines the data structures selected for the Traffic Management System project, including their justifications and complexity analysis for key operations. The design prioritizes efficiency while maintaining the flexibility needed for dynamic traffic control scenarios.