#include "UI.h"
#include <iostream>
using namespace std;

UI::UI()
{
    simulationMode = 0;
}
UI::~UI() {}
int UI::getSimulationMode()
{
    int Mode;
    cout << "=====================================================\n";
    cout << "   TRAFFIC CONTROL CENTER - SIMULATION SYSTEM\n";
    cout << "=====================================================\n";
    cout << endl;
    cout << "Select Simulation Mode:\n";
    cout << "1. Interactive Mode\n";
    cout << "2. Step-by-step Mode\n";
    cout << "3. Silent Mode\n";
    cout << endl;
    cout << "Enter your choice (1-3): ";
    cin >> Mode;
    cin.ignore(1000, '\n');
    while (Mode < 1 || Mode > 3)
    {
        cout << "Invalid choice. Enter number from 1, 2, or 3\n";
        cout << "Enter your choice (1-3): ";
        cin >> Mode;
        cin.ignore(1000, '\n');
    }
    simulationMode = Mode;
    return simulationMode;
}
int UI::selectTestFile()
{
    int choice;
    cout << "=====================================================\n";
    cout << "   SELECT INPUT TEST FILE\n";
    cout << "=====================================================\n";
    cout << endl;
    cout << "Available Test Files:\n";
    cout << "1. TestFile1.txt\n";
    cout << "2. TestFile2.txt\n";
    cout << "3. TestFile3.txt\n";
    cout << "4. TestFile4.txt\n";
    cout << "5. TestFile5.txt\n";
    cout << "6. TestFile6.txt\n";
    cout << endl;
    cout << "Enter your choice (1-6): ";
    cin >> choice;
    cin.ignore(1000, '\n');
    while (choice < 1 || choice > 6)
    {
        cout << "Invalid choice. Enter number from 1 to 6\n";
        cout << "Enter your choice (1-6): ";
        cin >> choice;
        cin.ignore(1000, '\n');
    }
    return choice;
}
string UI::getInputFilename()
{
    int choice = selectTestFile();
    if (choice >= 1 && choice <= 6) {
        string filename = "TestFile";
        char numberStr[10];
        sprintf_s(numberStr, "%d", choice);
        filename += numberStr;
        filename += ".txt";

        cout << "\nSelected: " << filename << endl;
        return filename;
    }

}
string UI::getOutputFilename()
{
    string filename;
    cout << "\n=====================================================\n";
    cout << "   OUTPUT FILE NAME\n";
    cout << "=====================================================\n";
    cout << endl;
    cout << "Enter output filename (e.g., Output1.txt): ";
    getline(cin, filename);
    if (filename.find(".txt") == string::npos) {
        filename += ".txt";
    }
    cout << "Output will be saved to: " << filename << endl;
    return filename;
}
void UI::printSimulationStart()
{
    switch (simulationMode) {
    case 1:
    case 2:
        cout << "\n=====================================================" << endl;
        cout << "          TRAFFIC CONTROL SIMULATION START" << endl;
        cout << "=====================================================" << endl;
        cout << endl;
        break;
    case 3:
        cout << "Silent Mode - Simulation Starts..." << endl;
        break;
    default:
        break;
    }
}
void UI::printEventHandled(Event* e)
{
    if (simulationMode == 3 || e == nullptr)
    {
        return;
    }
    cout << "Event: ";
    switch (e->getType())
    {
    case EVENT_ARRIVAL:
        if (e->getVehicle()) {
            cout << "Vehicle " << e->getVehicle()->getID()
                << " (" << e->getVehicle()->getTypeName() << ") "
                << "arrived at Intersection " << e->getVehicle()->getINT()
                << ", Lane " << e->getVehicle()->getLane();
        }
        break;
    case EVENT_CANCEL:
        cout << "Vehicle " << e->getVehicleID() << " canceled";
        break;
    case EVENT_ACCIDENT:
        cout << "Accident at Intersection " << e->getIntersectionID()
            << ", Lane " << e->getLaneID()
            << " for " << e->getDuration() << " timesteps";
        break;
    case EVENT_PROMOTION:
        cout << "Vehicle " << e->getVehicleID() << " promoted from PT to EV";
        break;
    case EVENT_ROAD_CLOSURE:
        cout << "Road closure at Intersection " << e->getIntersectionID()
            << " for " << e->getDuration() << " timesteps";
        break;
    default:
        break;
    }
    cout << endl;
}
void UI::printTimestep(int time)
{
    if (simulationMode == 3) return;

    cout << endl;
    cout << "Current Timestep: " << time << endl;
}
void UI::printQueuesState(Intersection** intersections, int numInt, int time)
{
    if (simulationMode == 3 || intersections == nullptr) return;

    for (int i = 0; i < numInt; i++) {
        if (intersections[i] == nullptr) continue;

        cout << "=============== INTERSECTION #" << intersections[i]->getID()
            << " ===============" << endl;
        if (intersections[i]->isClosed()) {
            cout << "[CLOSED - All lanes unavailable]" << endl;
            cout << "----------------------------------------------" << endl;
            continue;
        }
        cout << "EV queue: ";
        bool first = true;
        for (int laneId = 0; laneId < intersections[i]->getLaneCount(); laneId++) {
            const Lane* lane = intersections[i]->getLane(laneId);
            if (lane == nullptr) continue;

            const PriorityQueue<Vehicle*>& evQueue = lane->getEVQueue();
            Node<Vehicle*>* current = evQueue.getFront();
            while (current != nullptr) {
                if (!first) cout << ", ";
                cout << current->data->getID();
                first = false;
                current = current->next;
            }
        }
        if (first) cout << "(empty)";
        cout << endl;
        cout << "PT queue: ";
        first = true;
        for (int laneId = 0; laneId < intersections[i]->getLaneCount(); laneId++) {
            const Lane* lane = intersections[i]->getLane(laneId);
            if (lane == nullptr) continue;

            const Queue<Vehicle*>& ptQueue = lane->getPTQueue();
            Node<Vehicle*>* current = ptQueue.getFront();
            while (current != nullptr) {
                if (!first) cout << ", ";
                cout << current->data->getID();
                first = false;
                current = current->next;
            }
        }
        if (first) cout << "(empty)";
        cout << endl;
        cout << "NC queue: ";
        first = true;
        for (int laneId = 0; laneId < intersections[i]->getLaneCount(); laneId++) {
            const Lane* lane = intersections[i]->getLane(laneId);
            if (lane == nullptr) continue;

            const Queue<Vehicle*>& ncQueue = lane->getNCQueue();
            Node<Vehicle*>* current = ncQueue.getFront();
            while (current != nullptr) {
                if (!first) cout << ", ";
                cout << current->data->getID();
                first = false;
                current = current->next;
            }
        }
        if (first) cout << "(empty)";
        cout << endl;
        cout << "FV queue: ";
        first = true;
        for (int laneId = 0; laneId < intersections[i]->getLaneCount(); laneId++) {
            const Lane* lane = intersections[i]->getLane(laneId);
            if (lane == nullptr) continue;
            const Queue<Vehicle*>& fvQueue = lane->getFVQueue();
            Node<Vehicle*>* current = fvQueue.getFront();
            while (current != nullptr) {
                if (!first) cout << ", ";
                cout << current->data->getID();
                first = false;
                current = current->next;
            }
        }
        if (first) cout << "(empty)";
        cout << endl;
        int greenLane = intersections[i]->getGreenLane();
        cout << "Active Green Lane: ";
        if (greenLane != -1) {
            cout << "LN" << greenLane;
        }
        else {
            cout << "NONE";
        }
        cout << endl;
        cout << "Vehicles Crossing: ";
        bool anyCrossing = false;
        for (int laneID = 0; laneID < intersections[i]->getLaneCount(); laneID++) {
            Vehicle* crossing = intersections[i]->getCrossing(laneID);
            if (crossing != nullptr) {
                int remainingTime = intersections[i]->getRemainingCrossTime(laneID);
                if (anyCrossing) cout << ", ";
                cout << crossing->getID() << " (XD remaining = " << remainingTime << ")";
                anyCrossing = true;
            }
        }
        if (!anyCrossing) cout << "(none)";
        cout << endl;
        cout << "----------------------------------------------" << endl;
    }
}
void UI::printRerouting(int fromInt, int toInt, int vehicleCount) {
    if (simulationMode == 3) return;

    cout << "[REROUTING] " << vehicleCount << " vehicle(s) moved from Intersection "
        << fromInt << " to Intersection " << toInt << endl;
}
void UI::printAccidentBlocked(int intID, int laneID, int duration) {
    if (simulationMode == 3) return;

    cout << "[BLOCKED] Intersection " << intID << ", Lane " << laneID
        << " blocked for " << duration << " timesteps due to ACCIDENT" << endl;
}
void UI::printRoadClosure(int intID, int duration) {
    if (simulationMode == 3) return;

    cout << "[CLOSED] Intersection " << intID
        << " completely closed for " << duration << " timesteps" << endl;
}
void UI::waitForUser()
{
    if (simulationMode == 1)
    {
        cout << "Press Enter to display next timestep...";
        cin.get();
    }
    else if (simulationMode == 2)
    {
        for (volatile long i = 0; i < 100000000; i++) {

        }
    }

}
void UI::printSimulationEnd()
{
    if (simulationMode == 3) {
        cout << "Simulation Ends, Output File Created." << endl;
        return;
    }

    cout << endl;
    cout << "=====================================================" << endl;
    cout << "          SIMULATION COMPLETED" << endl;
    cout << "=====================================================" << endl;
    cout << "Output file has been generated successfully." << endl;
    cout << endl;
}