#ifndef TRAFFIC_CONTROL_CENTER_H
#define TRAFFIC_CONTROL_CENTER_H

#include <iostream>
#include <fstream>
#include "Vehicle.h"
#include "Event.h"
#include "Intersection.h"
#include "PriorityQueue.h"
#include "Queue.h"
#include "UI.h"

using namespace std;

class TrafficControlCenter {
private:
    int numIntersections;
    int switchingCost;
    int autoPromoteThreshold;
    int cancelThreshold;

    Intersection** intersections;
    PriorityQueue<Event*> eventQueue;
    Queue<Vehicle*> allVehicles;

    int currentTime;
    UI ui;
    int simulationMode;
    int* switchCountPerIntersection;
    Queue<Vehicle*> completedVehicles;
    Queue<Vehicle*> canceledVehicles;

    int totalAutoPromoted;
    int totalEV, totalPT, totalNC, totalFV;
    int totalCanceled;

    float avgWT_EV, avgWT_PT, avgWT_NC, avgWT_FV, avgWT_All;
    float avgXD_EV, avgXD_PT, avgXD_NC, avgXD_FV, avgXD_All;
    float percentAutoPromoted;
    float percentCanceled;

    int* intersectionBusyTime;
    Vehicle* mostDelayedVehicle;
    int maxWT;

    int* intersectionBlockedTime;
    int totalSimulationTime;
    float* percentTimeBlocked;
    bool reroutingEnabled;
    int reroutingPenalty;
    int** adjacencyMatrix;
public:
    TrafficControlCenter();
    ~TrafficControlCenter();

    bool loadFile(const char* filename);
    void initialize();

    void simulate(const char* outputFilename);

    void scheduleVehicles(int currentTime);
    void selectAndStartCrossing(Intersection* inter, int laneID, int currentTime);
    int findBestLaneToGreen(Intersection* inter, int currentTime);
    Vehicle* getHighestPriorityVehicle(Lane* lane, int currentTime);
    void recordSwitchAtIntersection(int intID);
    void decreaseAllBlockTimers();
    void processCrossingVehicles(int currentTime);
    bool hasActiveVehicles() const;

    void handleArrivalEvent(Event* e);
    void handleCancellationEvent(Event* e);
    void handlePromotionEvent(Event* e);
    void handleAccidentEvent(Event* e);
    void handleRoadClosureEvent(Event* e);
    void checkAutoPromotion(int currentTime);
    void checkAutoCancellation(int currentTime);
    void incrementWaitingTimes(int currentTime);
    int findNearestAvailableIntersection(int currentIntID);
    void rerouteVehiclesFromLane(int intID, int laneID);
    void rerouteVehiclesFromIntersection(int intID);
    int findAvailableLane(int intID);

    void recordVehicleCompletion(Vehicle* v, int completionTime);
    void recordVehicleCancellation(Vehicle* v);
    void finalizeStatistics();
    void writeOutputFile(const char* filename);

    Vehicle* findVehicle(int id);
    string getUIInputFilename() { return ui.getInputFilename(); }
    string getUIOutputFilename() { return ui.getOutputFilename(); }
};

#endif