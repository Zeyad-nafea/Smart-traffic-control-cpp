#include "Intersection.h"
#include <iostream>

Intersection::Intersection(int id, int numLanes, int switchCost)
    : intersectionID(id),
    laneCount(numLanes),
    switchingCost(switchCost),
    switchCount(0),         
    isOpen(true),           
    closureDuration(0),    
    currentGreenLane(-1),   
    switchCooldown(0)      
{

    lanes = new Lane[laneCount];
    crossingVehicles = new Vehicle * [laneCount];
    remainingCrossTime = new int[laneCount];

    // Initialize blocking & closure arrays
    laneBlocked = new bool[laneCount];
    blockDuration = new int[laneCount];

    for (int i = 0; i < laneCount; ++i) {
        lanes[i] = Lane(intersectionID, i);
        crossingVehicles[i] = nullptr;
        remainingCrossTime[i] = 0;
        laneBlocked[i] = false;        
        blockDuration[i] = 0;          
    }
}

Intersection::~Intersection() {
    delete[] lanes;
    delete[] crossingVehicles;
    delete[] remainingCrossTime;
    delete[] laneBlocked;
    delete[] blockDuration;
}

int Intersection::getID() const {
    return intersectionID;
}

int Intersection::getLaneCount() const {
    return laneCount;
}

int Intersection::getSwitchingCost() const {
    return switchingCost;
}

Lane* Intersection::getLane(int laneID) {
    if (laneID < 0 || laneID >= laneCount) return nullptr;
    return &lanes[laneID];
}

const Lane* Intersection::getLane(int laneID) const {
    if (laneID < 0 || laneID >= laneCount) return nullptr;
    return &lanes[laneID];
}

Vehicle* Intersection::getCrossing(int laneID) const {
    if (laneID < 0 || laneID >= laneCount) return nullptr;
    return crossingVehicles[laneID];
}

int Intersection::getRemainingCrossTime(int laneID) const {
    if (laneID < 0 || laneID >= laneCount) return 0;
    return remainingCrossTime[laneID];
}
bool Intersection::hasWaitingVehicles() const {
    for (int i = 0; i < laneCount; ++i) {
        if (!lanes[i].isEmpty()) {
            return true;
        }
    }
    return false;
}

int Intersection::getTotalWaitingVehicles() const {
    int total = 0;
    for (int i = 0; i < laneCount; ++i) {
        total += lanes[i].totalVehicles();
    }
    return total;
}

// (Acc) Lane Blocking 

void Intersection::blockLane(int laneID, int duration) {
    // Blocks a specified lane at given intersection for givem duration"
    if (laneID < 0 || laneID >= laneCount) return;
    laneBlocked[laneID] = true;
    blockDuration[laneID] = duration;
}

void Intersection::unblockLane(int laneID) {
    // unlock lane 
    if (laneID < 0 || laneID >= laneCount) return;
    laneBlocked[laneID] = false;
    blockDuration[laneID] = 0;
}

bool Intersection::isLaneBlocked(int laneID) const {
    // Prevents new vehicles from entering blocked lane while ACC is active
    if (laneID < 0 || laneID >= laneCount) return false;
    return laneBlocked[laneID];
}

//(RC) Intersection Closing

void Intersection::closeIntersection(int duration) {
    //Closes entire intersection for a specific duration
    isOpen = false;
    closureDuration = duration;
}

void Intersection::reopenIntersection() {
    // Reopens intersection
    isOpen = true;
    closureDuration = 0;
}

bool Intersection::isClosed() const {
    // All lanes become unavailable
    return !isOpen;
}

// Timer Management

void Intersection::decreaseBlockTimers() {
    // Auto decreasing block duration and auto unblocks when done
    for (int i = 0; i < laneCount; ++i) {
        if (laneBlocked[i] && blockDuration[i] > 0) {
            blockDuration[i]--;
            if (blockDuration[i] == 0) {
                laneBlocked[i] = false;
            }
        }
    }
}

void Intersection::decreaseClosureTimer() {
    // Auto decreasing closure duration and auto reopens when done
    if (!isOpen && closureDuration > 0) {
        closureDuration--;
        if (closureDuration == 0) {
            isOpen = true;
        }
    }
}

// Signal/Green Lane Methods

int Intersection::getGreenLane() const {
    return currentGreenLane;
}

void Intersection::setGreenLane(int laneID) {
    // Applies switching cost and cooldown when changing green lane
    if (laneID >= -1 && laneID < laneCount) {
        if (currentGreenLane != laneID && currentGreenLane != -1) {
            switchCooldown = switchingCost;
            incrementSwitchCount();
        }
        currentGreenLane = laneID;
    }
}

void Intersection::clearGreenLane() {
    currentGreenLane = -1;
}

// Switching Cost Management

int Intersection::getSwitchCooldown() const {
    return switchCooldown;
}

void Intersection::setSwitchCooldown(int cost) {
    switchCooldown = cost;
}

void Intersection::decreaseSwitchCooldown() {
    // Cooldown countdown
    if (switchCooldown > 0) {
        switchCooldown--;
    }
}

// Lane Locking for Freight Vehicles

bool Intersection::getLaneLocked(int laneID) const {
    if (laneID < 0 || laneID >= laneCount) return false;

    if (crossingVehicles[laneID] != nullptr &&
        crossingVehicles[laneID]->getType() == FV) {
        return true;
    }
    return false;
}
