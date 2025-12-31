#ifndef EVENT_H
#define EVENT_H

#include "Vehicle.h"
#include "PriorityQueue.h"
#include <string>
using namespace std;

enum EventType {
    EVENT_ARRIVAL,      // A: Vehicle arrival
    EVENT_CANCEL,       // X: Cancellation
    EVENT_PROMOTION,    // P: PT to EV promotion
    EVENT_ACCIDENT,     // ACC: Lane blockage
    EVENT_ROAD_CLOSURE  // RC: Intersection closure
};

class Event {
private:
    EventType type;
    int timestamp;
    Vehicle* vehicle;
    int vehicleID;
    int intersectionID;
    int laneID;
    int duration;

public:

    Event();
    Event(int time, Vehicle* v); // Arrival Event (A TYPE AT ID INT LN XD [PR])
    Event(EventType t, int time, int vID); // Cancel Event(X AT ID) and Promote Event(P AT ID)
    Event(int time, int intID, int ln, int dur); // Accident Event (ACC AT INT LN DUR)
    Event(int time, int intID, int dur); // Road Closed event:( RC AT INT DUR)
    EventType getType() const { return type; }
    int getTimestamp() const { return timestamp; }
    Vehicle* getVehicle() const { return vehicle; }
    int getVehicleID() const { return vehicleID; }
    int getIntersectionID() const { return intersectionID; }
    int getLaneID() const { return laneID; }
    int getDuration() const { return duration; }
    int getEventPriority() const;
    string getTypeString() const;
    int getCompositePriorty()const;



};

#endif // EVENT_H
