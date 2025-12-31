#ifndef VEHICLE_H
#define VEHICLE_H

#include <iostream>
#include <string>
using namespace std;

enum VehicleType { EV, PT, NC, FV };
enum Status { WAITING, PASSING, FINISHED, CANCELED };

class Vehicle {
private:
    int AT;
    int ID;
    int INT_ID;
    int LN;
    int XD;

    int WT;
    int CT;
    int AP;
    int CancelT;

    int Urgency;
    bool CargoFlag;

    VehicleType Type;
    Status status;

    bool promoted;

public:
    Vehicle();
    Vehicle(int at, int id, VehicleType type,
        int intersection, int lane, int xd,
        int urgency = 0, bool cargo = false,
        int autoPromote = 3, int autoCancel = 4);


    int getAT() const { return AT; }
    int getID() const { return ID; }
    int getINT() const { return INT_ID; }
    int getLane() const { return LN; }
    int getXD() const { return XD; }
    int getWT() const { return WT; }
    int getCT() const { return CT; }
    int getUrgency() const { return Urgency; }
    int getAP() const { return AP; }
    int getCancelT() const { return CancelT; }
    bool getCargoFlag() const { return CargoFlag; }
    VehicleType getType() const { return Type; }
    Status getStatus() const { return status; }
    bool isPromoted() const { return promoted; }


    void setWT(int w) { WT = w; }
    void setCT(int c) { CT = c; }
    void setStatus(Status s) { status = s; }
    void setType(VehicleType t) { Type = t; }
    void setINT(int intersection) { INT_ID = intersection; }
    void setLane(int lane) { LN = lane; }
    void markPromoted() { promoted = true; }

    string getTypeName() const;
    string getStatusName() const;

    ~Vehicle();
};

#endif