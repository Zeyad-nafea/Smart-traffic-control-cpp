#include "Event.h"

Event::Event()
{
    type = EVENT_ARRIVAL;
    timestamp = 0;
    vehicle = nullptr;
    vehicleID = -1;
    intersectionID = -1;
    laneID = -1;
    duration = 0;
}

string Event::getTypeString() const
{
    switch (type) {
    case EVENT_ARRIVAL:      return "ARRIVAL";
    case EVENT_CANCEL:       return "CANCEL";
    case EVENT_PROMOTION:    return "PROMOTION";
    case EVENT_ACCIDENT:     return "ACCIDENT";
    case EVENT_ROAD_CLOSURE: return "ROAD_CLOSURE";
    default:                 return "UNKNOWN";
    }
}

int Event::getEventPriority() const
{
    switch (type) {
    case EVENT_CANCEL:       return 1;
    case EVENT_PROMOTION:    return 2;
    case EVENT_ACCIDENT:     return 3;
    case EVENT_ROAD_CLOSURE: return 3;
    case EVENT_ARRIVAL:      return 4;
    default:                 return 5;
    }
}

Event::Event(int time, Vehicle* v) // Arrivale Event (A TYPE AT ID INT LN XD [PR])
{
    type = EVENT_ARRIVAL;
    timestamp = time;
    vehicle = v;
    vehicleID = -1;
    intersectionID = -1;
    laneID = -1;
    duration = 0;
}

Event::Event(EventType t, int time, int vID) // Cancel Event(X AT ID) and  Promote Event(P AT ID)
{
    type = t;
    timestamp = time;
    vehicle = nullptr;
    vehicleID = vID;
    intersectionID = -1;
    laneID = -1;
    duration = 0;
}

Event::Event(int time, int intID, int ln, int dur) // Accident Event (ACC AT INT LN DUR)
{
    type = EVENT_ACCIDENT;
    timestamp = time;
    vehicle = nullptr;
    vehicleID = -1;
    intersectionID = intID;
    laneID = ln;
    duration = dur;
}

Event::Event(int time, int intID, int dur) // Road Closed event:( RC AT INT DUR)
{
    type = EVENT_ROAD_CLOSURE;
    timestamp = time;
    vehicle = nullptr;
    vehicleID = -1;
    intersectionID = intID;
    laneID = -1;
    duration = dur;
}
int Event::getCompositePriorty()const
{
    return -1 * (timestamp * 100 + getEventPriority());
}


