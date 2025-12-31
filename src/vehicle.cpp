#include "Vehicle.h"

Vehicle::Vehicle()
    : AT(0), ID(0), INT_ID(0), LN(0), XD(0),
    WT(0), CT(-1),
    AP(3), CancelT(4),
    Urgency(0), CargoFlag(false),
    Type(NC), status(WAITING),
    promoted(false)
{
}

Vehicle::Vehicle(int at, int id, VehicleType type,
    int intersection, int lane, int xd,
    int urgency, bool cargo,
    int autoPromote, int autoCancel)
{
    AT = at;
    ID = id;
    Type = type;
    INT_ID = intersection;
    LN = lane;
    XD = xd;

    WT = 0;
    CT = -1;

    Urgency = urgency;
    CargoFlag = cargo;
    AP = autoPromote;
    CancelT = autoCancel;

    status = WAITING;
    promoted = false;
}

string Vehicle::getTypeName() const {
    switch (Type) {
    case EV: return "EV";
    case PT: return "PT";
    case NC: return "NC";
    case FV: return "FV";
    default: return "UNKNOWN";
    }
}

string Vehicle::getStatusName() const {
    switch (status) {
    case WAITING: return "WAITING";
    case PASSING: return "PASSING";
    case FINISHED: return "FINISHED";
    case CANCELED: return "CANCELED";
    default: return "UNKNOWN";
    }
}

Vehicle::~Vehicle() {}