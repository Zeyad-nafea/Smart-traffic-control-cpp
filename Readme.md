# Smart Traffic Control System

> **Event-driven traffic simulation** implementing priority-based vehicle scheduling, dynamic rerouting, and real-time intersection management using custom data structures in C++.

[![C++](https://img.shields.io/badge/C%2B%2B-11%2B-blue.svg)](https://isocpp.org/)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

---
## 👥 Team Members

| Name | Email |
|------|-------|
| **Zeyad Mohamed Fathy** | s-zeyad.nafea@zewailcity.edu.eg |
| **Omar Ahmed Fathy** | s-omar.elgazar@zewailcity.edu.eg |
| **Shaza Kazem Mahmoud** | s-shaza.mahmoud@zewailcity.edu.eg |
| **Ganna Allah Walid Helmy** | s-ganna.wahba@zewailcity.edu.eg |

---
## 🎯 Overview

A sophisticated traffic control center simulation that manages multiple intersections, handles dynamic events (accidents, road closures, vehicle arrivals), and prioritizes emergency vehicles while ensuring fairness across all vehicle types. Built entirely from scratch using custom data structures—**no STL containers**.

### Key Features

- **Priority-Based Scheduling**: Emergency vehicles (EV) preempt normal traffic using urgency-weighted priority queues
- **Dynamic Event Handling**: Real-time processing of arrivals, cancellations, promotions, accidents (ACC), and road closures (RC)
- **Adaptive Rerouting**: Automatic vehicle rerouting during lane blockages using adjacency-based pathfinding
- **Auto-Promotion System**: Public transport (PT) automatically promoted to EV status after waiting threshold
- **Fair Cancellation**: Normal cars (NC) auto-cancel after exceeding wait limits to prevent indefinite blocking
- **Multi-Lane Management**: Each intersection supports 4 lanes with independent queuing and switching cost optimization
- **Comprehensive Statistics**: Detailed performance metrics including wait times, completion times, and intersection utilization

---

## 🏗️ Architecture

### Core Components

```
TrafficControlCenter (Orchestrator)
├── Event Queue (Priority Queue)
│   ├── Arrival Events
│   ├── Cancellation Events
│   ├── Promotion Events
│   ├── Accident Events
│   └── Road Closure Events
│
├── Intersections (Array)
│   └── Lanes (Per Intersection)
│       ├── EV Queue (Priority Queue)
│       ├── PT Queue (FIFO Queue)
│       ├── NC Queue (FIFO Queue)
│       └── FV Queue (FIFO Queue)
│
└── Vehicle Registry (Queue)
    ├── All Vehicles
    ├── Completed Vehicles
    └── Canceled Vehicles
```

### Vehicle Types & Priority

| Type | Priority | Behavior |
|------|----------|----------|
| **EV** (Emergency) | Highest | Can interrupt green signals; Priority = (Urgency × 2) - WT |
| **PT** (Public Transport) | Medium | FIFO within lane; Auto-promotes to EV if WT ≥ AP threshold |
| **FV** (Freight) | Medium-Low | Locks lane during crossing; Cannot be interrupted |
| **NC** (Normal Cars) | Lowest | FIFO; Auto-cancels if WT ≥ CancelT threshold |

---

## 🚀 Implementation Highlights

### Custom Data Structures (No STL)

- **Priority Queue**: Max-heap implementation for event scheduling and EV prioritization
- **Queue**: FIFO implementation for PT, NC, FV vehicle management
- **Node-based Linked Lists**: Memory-efficient pointer sharing (no copying)
- **Adjacency Matrix**: Intersection connectivity for rerouting pathfinding

### Event-Driven Simulation

Events processed in strict order per timestep:
1. **Cancellations** (highest priority)
2. **Promotions**
3. **Accidents & Road Closures**
4. **Arrivals** (lowest priority)
5. Lane assignment and green signal updates

### Scheduling Algorithm

```cpp
// EV Priority Calculation
priority = (urgency * 2) - waiting_time;

// Lane Selection Logic
for each lane:
  if (has_EV):
    select_lane_with_highest_EV_priority()
  else if (has_PT):
    select_lane_with_oldest_PT()
  else if (has_FV):
    select_lane_with_oldest_FV()
  else:
    select_lane_with_oldest_NC()
```

---

## 📊 Simulation Modes

| Mode | Description | Use Case |
|------|-------------|----------|
| **Interactive** | Manual step-through with console output | Debugging, demonstrations |
| **Step-by-Step** | Auto-advances with delay | Presentations, live monitoring |
| **Silent** | No console output, file-only | Performance testing, batch processing |

---

## 🛠️ Building & Running

### Prerequisites
- C++11 or higher
- Any standard C++ compiler (GCC, Clang, MSVC)

### Compilation
```bash
# Using g++
g++ -std=c++11 src/*.cpp -o traffic_sim

# Using Visual Studio (already configured via .vcxproj)
# Open in VS and build solution
```

### Execution
```bash
./traffic_sim
```

The program will prompt you to:
1. Select simulation mode (1-3)
2. Choose input test file (1-6)
3. Specify output filename

---

## 📁 Input File Format

```
<num_intersections>
<switching_cost>
<auto_promote_threshold>
<cancel_threshold>
<rerouting_flag: ON/OFF>
Connections:
<intersection_id>: <connected_intersection_ids>
<num_events>
<event_lines>
```

### Event Syntax

| Event Type | Format | Example |
|------------|--------|---------|
| **Arrival** | `A TYPE AT ID INT LN XD [PR]` | `A EV 3 301 1 2 2 5` |
| **Cancellation** | `X AT ID` | `X 6 101` |
| **Promotion** | `P AT ID` | `P 5 201` |
| **Accident** | `ACC AT INT LN DUR` | `ACC 4 1 1 3` |
| **Road Closure** | `RC AT INT DUR` | `RC 8 2 2` |

---

## 📈 Output Statistics

The system generates comprehensive metrics:

- **Vehicle Counts**: Total and per-type breakdown (EV, PT, NC, FV)
- **Average Wait Times**: Overall and per vehicle type
- **Average Crossing Duration**: System-wide XD analysis
- **Auto-Promotion Rate**: % of PT vehicles promoted to EV
- **Signal Switches**: Total lane changes across all intersections
- **Cancellation Rate**: % of vehicles that timed out
- **Most Delayed Vehicle**: ID and wait time of longest-waiting vehicle
- **Intersection Blockage**: % of time each intersection was blocked (ACC/RC)

---

## 🧪 Test Cases

Six comprehensive test files included:

| Test File | Vehicles | Intersections | Events | Focus Area |
|-----------|----------|---------------|--------|------------|
| `TestFile1.txt` | 20-30 | 2-4 | 8-10 | Basic arrivals, promotions |
| `TestFile2.txt` | 40-60 | 4-6 | 15-20 | Cancellations, accidents |
| `TestFile3.txt` | 80-100 | 6-8 | 25-30 | Road closures, rerouting |
| `TestFile4.txt` | 100+ | 8-10 | 35+ | Heavy load, EV priority |
| `TestFile5.txt` | 150+ | 10+ | 50+ | Stress test, edge cases |
| `TestFile6.txt` | 200+ | 12+ | 75+ | Max capacity, all features |

---

## 🎓 Academic Context

**Course**: CSAI 201 - Data Structures and Algorithms  
**Institution**: Zewail City University  
**Semester**: Fall 2025

### Project Requirements Met

✅ Custom data structures (Queue, Priority Queue, Linked Lists)  
✅ No global variables, no STL, no friendship  
✅ Pointer-based memory sharing (zero-copy architecture)  
✅ Object-oriented design with clear separation of concerns  
✅ Event-driven simulation with discrete timestep advancement  
✅ Comprehensive statistics and output file generation  
✅ Multiple simulation modes for different use cases  

---

## 📝 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

---

## 🔍 Technical Decisions

### Why Custom Data Structures?

This project deliberately avoids STL containers to demonstrate:
- **Deep understanding** of data structure internals
- **Memory management** expertise (manual allocation, pointer manipulation)
- **Performance optimization** (cache locality, minimal allocations)
- **Academic integrity** (no black-box dependencies)

### Design Patterns Used

- **Facade Pattern**: `TrafficControlCenter` abstracts complexity from UI
- **Strategy Pattern**: Different vehicle types implement varying priority logic
- **Observer Pattern**: Events trigger state changes across intersections
- **State Pattern**: Vehicle status transitions (WAITING → PASSING → FINISHED)

---

**Note**: This is an academic project. The simulation logic is designed for educational purposes and does not reflect real-world traffic control systems.
