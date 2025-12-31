#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "lanes.h"
#include "Vehicle.h"

class Intersection {
private:
    int intersectionID;
    Lane* lanes;
    int laneCount;
    int switchingCost;
    int switchCount;            
    bool* laneBlocked;           
    int* blockDuration;     
    bool isOpen;                 
    int closureDuration;        
    int currentGreenLane;      
    int switchCooldown;         
public:
    Intersection(int id, int numLanes, int switchCost = 2);
    ~Intersection();
    Vehicle** crossingVehicles;  
    int* remainingCrossTime;     

    // Getters
    int getID() const;
    int getLaneCount() const;
    int getSwitchingCost() const;
    int getSwitchCount() const { return switchCount; }
    Lane* getLane(int laneID);
    const Lane* getLane(int laneID) const;
    bool hasWaitingVehicles() const;
    int getTotalWaitingVehicles() const;

    // Access Methods for UI timestamps
    Vehicle* getCrossing(int laneID) const;
    int getRemainingCrossTime(int laneID) const;

    // Accident Event (ACC) Lane Blocking
    void blockLane(int laneID, int duration);
    void unblockLane(int laneID);
    bool isLaneBlocked(int laneID) const;

    // Road Closure Event (RC) Intersection Closing
    void closeIntersection(int duration);
    void reopenIntersection();
    bool isClosed() const;

    // Called each timestep
    void decreaseBlockTimers();     
    void decreaseClosureTimer();     

    // Signal/Green Lane Management 
    int getGreenLane() const;
    void setGreenLane(int laneID);
    void clearGreenLane();

    // Switching Cost Management 
    int getSwitchCooldown() const;
    void setSwitchCooldown(int cost);
    void decreaseSwitchCooldown();
    void incrementSwitchCount() { switchCount++; }

    // Lane Locking for Freight Vehicles 
    bool getLaneLocked(int laneID) const;
};

#endif