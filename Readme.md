# Data Structures and Algorithms - Project Phase 1.1 (Revised)

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

## System Architecture Overview

The system follows a hierarchical structure:
```
City (Simulation)
└── Traffic Control Center (TCC)
    ├── Event Queue (Global)
    ├── Rerouting Manager
    └── Intersections Array
        └── Intersection (each)
            ├── Lanes Array
            │   └── Lane (each)
            │       ├── EV_Queue (Priority Queue)
            │       ├── PT_Queue (Queue)
            │       ├── NC_Queue (Queue)
            │       └── FV_Queue (Queue)
            └── Crossing Vehicles Array
```

## Proposed Data Structures

### 1. Traffic Control Center (TCC)

#### Event Queue
- **Data Structure:** Priority Queue
- **Reason:** Manages all simulation events ordered by timestamp; handles events with same timestamp according to priority order
- **Event Priority Order (same timestamp):**
  1. Vehicle arrivals
  2. Cancellations
  3. Promotions
  4. Accidents
  5. Lane closures
  6. Crossing completions
- **Operations:**
  - `InsertEvent(event)` - O(log n)
  - `GetNextEvent()` - O(log n)
  - `IsEmpty()` - O(1)
  - `PeekNextEventTime()` - O(1)

#### Intersections Array
- **Data Structure:** Static Array
- **Reason:** Fixed number of intersections in the city
- **Operations:**
  - `GetIntersection(id)` - O(1)
  - `GetAllIntersections()` - O(1)
  - `UpdateIntersectionStatus(id)` - O(1)

#### TCC Core Operations
- `ProcessNextEvent()` - O(log n + event_processing_cost)
- `AdvanceSimulationTime()` - O(1)
- `GetCurrentTime()` - O(1)
- `TriggerRerouting(intersection_id)` - O(V + E)
- `CollectStatistics()` - O(n)

### 2. Intersection Class

#### Lanes Array
- **Data Structure:** Static Array
- **Reason:** Each intersection has a fixed number of lanes
- **Operations:**
  - `GetLane(lane_id)` - O(1)
  - `SwitchActiveLane()` - O(1)
  - `GetActiveLane()` - O(1)
  - `DisplayAllLanes()` - O(n)

#### Crossing Vehicles Array
- **Data Structure:** Array indexed by Lane ID
- **Reason:** Tracks vehicles currently crossing in each lane
- **Operations:**
  - `AddCrossingVehicle(lane_id, vehicle)` - O(1)
  - `RemoveCrossingVehicle(lane_id)` - O(1)
  - `UpdateRemainingTime(lane_id)` - O(1)
  - `GetCrossingVehicle(lane_id)` - O(1)

#### Intersection State Management
- **Data Structure:** Enumeration/Status Flag
- **States:** NORMAL, ACCIDENT, LANE_CLOSED
- **Operations:**
  - `GetStatus()` - O(1)
  - `SetStatus(status)` - O(1)
  - `HandleAccident(lane_id)` - O(1)
  - `CloseLane(lane_id)` - O(1)
  - `ReopenLane(lane_id)` - O(1)

### 3. Lane Class

#### Vehicle Queues (Per Lane)

##### EV_Queue (Electric Vehicle Queue)
- **Data Structure:** Priority Queue
- **Reason:** EVs have highest priority and may have different priority levels
- **Operations:**
  - `Enqueue(vehicle)` - O(log n)
  - `Dequeue()` - O(log n)
  - `Peek()` - O(1)
  - `IsEmpty()` - O(1)
  - `Size()` - O(1)

##### PT_Queue (Public Transport Queue)
- **Data Structure:** Queue
- **Reason:** First-come-first-served with auto-promotion when WT ≥ AP
- **Operations:**
  - `Enqueue(vehicle)` - O(1)
  - `Dequeue()` - O(1)
  - `PromoteIfWaitingTooLong(current_time)` - O(1)
  - `Peek()` - O(1)
  - `IsEmpty()` - O(1)
  - `Size()` - O(1)

##### NC_Queue (Normal Car Queue)
- **Data Structure:** Queue
- **Reason:** First-come-first-served with cancellation check when WT ≥ CancelT
- **Operations:**
  - `Enqueue(vehicle)` - O(1)
  - `Dequeue()` - O(1)
  - `CheckCancellation(current_time)` - O(1)
  - `Peek()` - O(1)
  - `IsEmpty()` - O(1)
  - `Size()` - O(1)

##### FV_Queue (Fire Vehicle Queue)
- **Data Structure:** Queue
- **Reason:** Emergency vehicles, longer crossing time, locks lane
- **Operations:**
  - `Enqueue(vehicle)` - O(1)
  - `Dequeue()` - O(1)
  - `Peek()` - O(1)
  - `IsEmpty()` - O(1)
  - `Size()` - O(1)

#### Lane State Management
- **Data Structure:** Enumeration/Flags
- **States:** OPEN, BLOCKED, CLOSED, LOCKED_BY_FV
- **Operations:**
  - `GetStatus()` - O(1)
  - `SetStatus(status)` - O(1)
  - `IsBlocked()` - O(1)
  - `LockLane()` - O(1)
  - `UnlockLane()` - O(1)
  - `GetWaitingVehicleCount()` - O(1)
  - `GetTotalWaitingVehicles()` - O(1)

#### Lane Timing
- **Data Structure:** Integer counters
- **Operations:**
  - `GetGreenTimeRemaining()` - O(1)
  - `SetGreenTime(time)` - O(1)
  - `DecrementGreenTime()` - O(1)

### 4. Rerouting Manager

#### Adjacency Map (Road Network)
- **Data Structure:** Array of Linked Lists (Adjacency List)
- **Reason:** Represents road network; each intersection stores connections to neighbors
- **Operations:**
  - `AddConnection(from, to)` - O(1)
  - `RemoveConnection(from, to)` - O(k) where k = number of neighbors
  - `GetNeighbors(intersection_id)` - O(1)
  - `IsConnected(from, to)` - O(k)

#### Blocked Lanes/Intersections List
- **Data Structure:** Dynamic Array or Hash Set
- **Reason:** Tracks currently blocked paths for rerouting decisions
- **Operations:**
  - `AddBlockedPath(intersection_id, lane_id)` - O(1)
  - `RemoveBlockedPath(intersection_id, lane_id)` - O(1)
  - `IsPathBlocked(intersection_id, lane_id)` - O(1)

#### Rerouting Queue (BFS/DFS traversal)
- **Data Structure:** Queue
- **Reason:** Used during pathfinding to explore alternative routes
- **Operations:**
  - `Enqueue(intersection_id)` - O(1)
  - `Dequeue()` - O(1)
  - `IsEmpty()` - O(1)

#### Rerouting Operations
- `FindAlternatePath(from, to, blocked_paths)` - O(V + E)
- `UpdateBlockedStatus(intersection_id, lane_id, blocked)` - O(1)
- `GetReroutedPath(vehicle)` - O(V + E)
- `NotifyTCC(intersection_id)` - O(1)

### 5. Vehicle Class
- **Data Structure:** Object with attributes
- **Attributes:**
  - ID, Type, ArrivalTime, WaitingTime, CrossingTime, Priority
  - SourceIntersection, DestinationIntersection, CurrentLane
- **Operations:**
  - `UpdateWaitingTime()` - O(1)
  - `GetType()` - O(1)
  - `ShouldPromote(current_time, threshold)` - O(1)
  - `ShouldCancel(current_time, threshold)` - O(1)

## Event Processing Flow

1. **TCC** retrieves next event from Event Queue
2. Based on event type:
   - **Arrival:** Vehicle added to appropriate lane queue at destination intersection
   - **Cancellation:** Vehicle removed from NC_Queue if WT ≥ CancelT
   - **Promotion:** PT vehicle moved from PT_Queue to EV_Queue if WT ≥ AP
   - **Accident:** Intersection status updated, Rerouting Manager activated
   - **Closure:** Lane marked as closed, vehicles rerouted
   - **Crossing Complete:** Vehicle removed from crossing array, next vehicle processes
3. **TCC** updates affected intersections and schedules new events as needed

## Rerouting Manager Integration

### Trigger Conditions
- Accident occurs at intersection
- Lane closure event
- Intersection becomes blocked

### Interaction Flow
1. **Intersection** detects accident/closure → notifies **TCC**
2. **TCC** calls **Rerouting Manager** with affected intersection/lane
3. **Rerouting Manager**:
   - Updates blocked paths in its data structures
   - Queries intersection for waiting vehicles needing rerouting
   - Uses BFS/DFS with adjacency list to find alternative paths
   - Returns alternative routes to **TCC**
4. **TCC** schedules new arrival events for rerouted vehicles at alternative intersections
5. Vehicles removed from blocked queues and added to alternative path queues

## Data Flow Diagram

```
Event Queue → TCC (Process Event)
                ↓
         [Check Event Type]
                ↓
    ┌───────────┴───────────┐
    ↓                       ↓
Intersection          Rerouting Manager
    ↓                       ↓
  Lane                  Adjacency Map
    ↓                       ↓
Vehicle Queues        Find Alternative Path
(EV/PT/NC/FV)              ↓
    ↓                  Update Routes
Process Vehicle            ↓
    ↓              Notify TCC (Schedule)
Crossing Array             ↓
    ↓              New Arrival Events
Complete Event             ↓
    ↓                Event Queue
Schedule Next
```

## Summary

This revised design addresses:
- ✅ Hierarchical organization: queues inside lanes inside intersections
- ✅ Traffic Control Center as central coordinator
- ✅ Event handling priority order
- ✅ Rerouting Manager integration with TCC and intersections
- ✅ Complete system hierarchy from city level to vehicle level
- ✅ Proper data structure justifications with Big O analysis

The design now reflects both the system hierarchy and simulation logic as required by the project specifications.
