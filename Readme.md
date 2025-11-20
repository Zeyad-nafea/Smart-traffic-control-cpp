# Data Structures and Algorithms
## Project Phase 1.1 Report
### Spring 2025

**Team#: 14**  
**Team Email:** s-zeyad.nafea@zewailcity.edu.eg

---

### Members' Info:

| **Member Name** | **ID** | **Email** |
|-----------------|--------|-----------|
| Zeyad Mohamed Fathy | 202400354 | s-zeyad.nafea@zewailcity.edu.eg |
| Omar Ahmed Fathy | 202401823 | s-omar.elgazar@zewailcity.edu.eg |
| Shaze Kazem Mahmoud | 202400948 | s-shaza.mahmoud@zewailcity.edu.eg |
| Ganna Allah Walid Helmy | 202401020 | s-ganna.wahba@zewailcity.edu.eg |

---

## Proposed Data Structures

| **List Name** | **Inside Class** | **Chosen DS / Reason / Additional Functions** | **List Operations & BIG O Analysis** |
|---------------|------------------|-----------------------------------------------|--------------------------------------|
| **Event_Queue** | TCC (Traffic Control Center) | **Priority Queue**<br><br>Events must be processed in chronological order by timestamp. When multiple events occur at the same timestamp, they follow priority order: Cancellations (1) → Promotions (2) → Accidents/Closures (3) → Arrivals (4) → Completions (5).<br><br>Priority Queue ensures O(log n) insertion and extraction while maintaining order. | **InsertEvent()** = O(log n)<br>**GetNextEvent()** = O(log n)<br>**PeekNextEvent()** = O(1)<br>**IsEmpty()** = O(1) |
| **Intersections_Array** | TCC | **Static Array**<br><br>The number of intersections is fixed and known at simulation start. Array provides direct O(1) access to any intersection by ID. | **GetIntersection(id)** = O(1)<br>**UpdateStatus(id)** = O(1)<br>**GetCount()** = O(1) |
| **EV_Queue** | Lane | **Priority Queue (Derived)**<br><br>Base: Priority Queue<br><br>EVs have highest priority and use formula: Priority = (Urgency × 2) - WT. Higher priority EVs must be served first.<br><br>**Additional Functions:**<br>- CalculatePriority(vehicle): computes (Urgency × 2) - WT<br>- UpdatePriorities(currentTime): recalculates WT for all waiting EVs | **Enqueue()** = O(log n)<br>**Dequeue()** = O(log n)<br>**Peek()** = O(1)<br>**IsEmpty()** = O(1)<br>**Size()** = O(1)<br>**CalculatePriority()** = O(1)<br>**UpdatePriorities()** = O(n) |
| **PT_Queue** | Lane | **Queue**<br><br>Public Transport follows FCFS order. When WT ≥ AutoPromote threshold, PT is promoted to EV_Queue. FIFO ensures fairness among PT vehicles. | **Enqueue()** = O(1)<br>**Dequeue()** = O(1)<br>**Peek()** = O(1)<br>**IsEmpty()** = O(1)<br>**Size()** = O(1) |
| **NC_Queue** | Lane | **Queue**<br><br>Normal Cars follow FCFS order. When WT ≥ CancelThreshold, vehicle is removed (canceled). FIFO maintains arrival order. | **Enqueue()** = O(1)<br>**Dequeue()** = O(1)<br>**Peek()** = O(1)<br>**IsEmpty()** = O(1)<br>**Size()** = O(1) |
| **FV_Queue** | Lane | **Queue**<br><br>Freight Vehicles follow FCFS order. They lock lanes during crossing due to longer crossing duration. FIFO is sufficient. | **Enqueue()** = O(1)<br>**Dequeue()** = O(1)<br>**Peek()** = O(1)<br>**IsEmpty()** = O(1)<br>**Size()** = O(1) |
| **Lanes_Array** | Intersection | **Static Array**<br><br>Each intersection has a fixed number of lanes. Array provides O(1) access to any lane. | **GetLane(id)** = O(1)<br>**GetActiveLane()** = O(1)<br>**SwitchLane()** = O(1)<br>**GetLaneCount()** = O(1) |
| **Crossing_Vehicles** | Intersection | **Static Array (indexed by Lane ID)**<br><br>Tracks the vehicle currently crossing in each lane. Only one vehicle crosses per lane at a time, so array indexed by lane ID gives O(1) access. | **AddCrossing(laneID, vehicle)** = O(1)<br>**RemoveCrossing(laneID)** = O(1)<br>**GetCrossing(laneID)** = O(1)<br>**UpdateTime(laneID)** = O(1) |
| **Adjacency_Map** | Rerouting Manager | **Array of Linked Lists (Adjacency List)**<br><br>Represents road network between intersections. Each intersection stores list of connected neighbors. Linked lists allow dynamic connections and O(1) insertion. | **AddConnection(from, to)** = O(1)<br>**GetNeighbors(id)** = O(1)<br>**IsConnected(from, to)** = O(k), k = neighbors<br>**RemoveConnection(from, to)** = O(k) |
| **Blocked_Paths** | Rerouting Manager | **Dynamic Array** (or Stack-based structure)<br><br>Tracks temporarily blocked lanes/intersections during accidents or closures. Dynamic array allows additions and removals as blockages occur and clear. | **AddBlocked(intID, laneID)** = O(1)<br>**RemoveBlocked(intID, laneID)** = O(n)<br>**IsBlocked(intID, laneID)** = O(n)<br>**ClearAll()** = O(1) |
| **BFS_Queue** | Rerouting Manager | **Queue**<br><br>Used during pathfinding (BFS) to explore alternative routes when lanes are blocked. FIFO order ensures breadth-first exploration of the road network. | **Enqueue()** = O(1)<br>**Dequeue()** = O(1)<br>**IsEmpty()** = O(1)<br>**Size()** = O(1) |
| **Visited_Array** | Rerouting Manager | **Boolean Array**<br><br>Prevents revisiting intersections during BFS pathfinding. Size equals number of intersections. Array provides O(1) access for marking and checking visited status. | **MarkVisited(id)** = O(1)<br>**IsVisited(id)** = O(1)<br>**Reset()** = O(n) |

---

## Notes:
- All queues (EV_Queue, PT_Queue, NC_Queue, FV_Queue, BFS_Queue) and priority queues (Event_Queue, EV_Queue) are derived from the base Queue/Priority Queue classes provided in labs.
- EV_Queue is the only derived class with additional functions for priority calculation.

