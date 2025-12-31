#ifndef LANE_H
#define LANE_H

#include "Queue.h"
#include "PriorityQueue.h"
#include "Vehicle.h"


class Lane {
private:
    int intersectionID;
    int laneID;

    PriorityQueue<Vehicle*> EV_queue;
    Queue<Vehicle*> PT_queue;
    Queue<Vehicle*> NC_queue;
    Queue<Vehicle*> FV_queue;

    bool blocked;
    int remainingBlockedDuration;
    bool laneLocked;

public:


    Lane();
    Lane(int intID, int lID);
    ~Lane();



    int getIntersectionID() const;
    int getLaneID() const;
    bool isBlocked() const;
    bool isLaneLocked() const;
    bool canEnterLane() const;


    void blockLane(int duration);


    void unblockLane();


    void updateBlockedLane();


    void lockLane();

    void unlockLane();


    void addVehicle(Vehicle* v, int priority = 0);

    Vehicle* peek() const;


    Vehicle* dequeueFront();


    bool isEmpty() const;


    int totalVehicles() const;


    Vehicle* findVehicleByID(int id) const;


    bool removeVehicle(Vehicle* v);


    void updateAllWaitingTimes(int currentTime);


    void recalculateEVPriorities(int currentTime);


    const PriorityQueue<Vehicle*>& getEVQueue() const;
    const Queue<Vehicle*>& getPTQueue() const;
    const Queue<Vehicle*>& getNCQueue() const;
    const Queue<Vehicle*>& getFVQueue() const;


    int getQueueSize(VehicleType type) const;


    void printLaneState() const;
};

#endif 