#ifndef UI_H
#define UI_H

#include "Event.h"
#include "Vehicle.h"
#include "intersection.h"
#include <string>
using namespace std;

class UI
{
private:
    int simulationMode;
public:
    UI();
    ~UI();
    int getSimulationMode();
    string getInputFilename();
    string getOutputFilename();
    void printSimulationStart();
    void printTimestep(int time);
    void printEventHandled(Event* e);
    void printQueuesState(Intersection** intersections, int numInt, int time);
    void printSimulationEnd();
    void waitForUser();
    int selectTestFile();
    void printRerouting(int fromInt, int toInt, int vehicleCount);
    void printAccidentBlocked(int intID, int laneID, int duration);
    void printRoadClosure(int intID, int duration);

};

#endif