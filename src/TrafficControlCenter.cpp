#include "TrafficControlCenter.h"
#include <sstream> 
TrafficControlCenter::TrafficControlCenter()
    : numIntersections(0), switchingCost(0),
    autoPromoteThreshold(0), cancelThreshold(0),
    intersections(nullptr), currentTime(0),
    switchCountPerIntersection(nullptr),
    totalAutoPromoted(0), totalEV(0), totalPT(0), totalNC(0), totalFV(0),
    totalCanceled(0),
    avgWT_EV(0), avgWT_PT(0), avgWT_NC(0), avgWT_FV(0), avgWT_All(0),
    avgXD_EV(0), avgXD_PT(0), avgXD_NC(0), avgXD_FV(0), avgXD_All(0),
    percentAutoPromoted(0), percentCanceled(0),
    intersectionBusyTime(nullptr),
    reroutingEnabled(false), reroutingPenalty(0),
    simulationMode(0), adjacencyMatrix(nullptr),
    mostDelayedVehicle(nullptr), maxWT(0),
    intersectionBlockedTime(nullptr),
    totalSimulationTime(0),
    percentTimeBlocked(nullptr)
{
}
TrafficControlCenter::~TrafficControlCenter() {
    if (intersections) {
        for (int i = 0; i < numIntersections; i++) {
            delete intersections[i];
        }
        delete[] intersections;
    }
    if (switchCountPerIntersection) {
        delete[] switchCountPerIntersection;
    }
    if (intersectionBusyTime) {
        delete[] intersectionBusyTime;
    }
    while (!allVehicles.isEmpty()) {
        Vehicle* v = allVehicles.dequeue();
        delete v;
    }
    while (!eventQueue.isEmpty()) {
        Event* e = eventQueue.dequeue();
        delete e;
    }
    while (!completedVehicles.isEmpty()) {
        completedVehicles.dequeue();
    }
    while (!canceledVehicles.isEmpty()) {
        canceledVehicles.dequeue();
    }
    if (adjacencyMatrix) {
        for (int i = 0; i < numIntersections; i++) {
            delete[] adjacencyMatrix[i];
        }
        delete[] adjacencyMatrix;
    }
    if (intersectionBlockedTime) {
        delete[] intersectionBlockedTime;
    }
    if (percentTimeBlocked) {
        delete[] percentTimeBlocked;
    }
}
bool TrafficControlCenter::loadFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file >> numIntersections >> switchingCost >> autoPromoteThreshold >> cancelThreshold;

    if (numIntersections <= 0 || numIntersections > 100) {
        cerr << "ERROR: Invalid number of intersections!\n";
        return false;
    }
    string reroutingFlag;
    file >> reroutingFlag;
    if (reroutingFlag == "ON") {
        reroutingEnabled = true;
        reroutingPenalty = 2;
    }
    else {
        reroutingEnabled = false;
        reroutingPenalty = 0;
    }
    string connectionsLabel;
    file >> connectionsLabel;
    adjacencyMatrix = new int* [numIntersections];
    for (int i = 0; i < numIntersections; i++) {
        adjacencyMatrix[i] = new int[numIntersections];
        for (int j = 0; j < numIntersections; j++) {
            adjacencyMatrix[i][j] = 0;
        }
    }
    for (int i = 0; i < numIntersections; i++) {
        int intersectionNum;
        char colon;
        file >> intersectionNum >> colon;
        string line;
        getline(file, line);
        istringstream iss(line);
        int connectedTo;
        while (iss >> connectedTo) {
            adjacencyMatrix[intersectionNum - 1][connectedTo - 1] = 1;
            adjacencyMatrix[connectedTo - 1][intersectionNum - 1] = 1;
        }
    }
    intersections = new Intersection * [numIntersections];
    for (int i = 0; i < numIntersections; i++) {
        intersections[i] = new Intersection(i + 1, 4, switchingCost);
    }
    int numEvents;
    file >> numEvents;
    for (int i = 0; i < numEvents; i++) {
        string eventType;
        file >> eventType;
        if (eventType == "A") {
            string typeStr;
            int at, id, intID, lane, xd, pr = 0;

            file >> typeStr >> at >> id >> intID >> lane >> xd;

            VehicleType vType = NC;
            if (typeStr == "EV") vType = EV;
            else if (typeStr == "PT") vType = PT;
            else if (typeStr == "NC") vType = NC;
            else if (typeStr == "FV") vType = FV;

            if (vType == EV) {
                file >> pr;
            }
            Vehicle* v = new Vehicle(at, id, vType, intID, lane, xd, pr, false,
                autoPromoteThreshold, cancelThreshold);
            allVehicles.enqueue(v);
            Event* e = new Event(at, v);
            int priority = -(at * 10 + 4);
            eventQueue.enqueue(e, priority);
        }
        else if (eventType == "X") {
            int at, id;
            file >> at >> id;
            Event* e = new Event(EVENT_CANCEL, at, id);
            int priority = -(at * 10 + 1);
            eventQueue.enqueue(e, priority);
        }
        else if (eventType == "P") {
            int at, id;
            file >> at >> id;

            Event* e = new Event(EVENT_PROMOTION, at, id);
            int priority = -(at * 10 + 2);
            eventQueue.enqueue(e, priority);
        }
        else if (eventType == "ACC") {
            int at, intID, ln, dur;
            file >> at >> intID >> ln >> dur;

            Event* e = new Event(at, intID, ln, dur);
            int priority = -(at * 10 + 3);
            eventQueue.enqueue(e, priority);
        }
        else if (eventType == "RC") {
            int at, intID, dur;
            file >> at >> intID >> dur;
            Event* e = new Event(at, intID, dur);
            int priority = -(at * 10 + 3);
            eventQueue.enqueue(e, priority);
        }
    }

    file.close();
    return true;
}
void TrafficControlCenter::initialize() {
    currentTime = 0;
    switchCountPerIntersection = new int[numIntersections];
    intersectionBusyTime = new int[numIntersections];
    intersectionBlockedTime = new int[numIntersections];
    percentTimeBlocked = new float[numIntersections];

    for (int i = 0; i < numIntersections; i++) {
        switchCountPerIntersection[i] = 0;
        intersectionBusyTime[i] = 0;
        intersectionBlockedTime[i] = 0;
        percentTimeBlocked[i] = 0.0f;
    }
    mostDelayedVehicle = nullptr;
    maxWT = 0;
    totalSimulationTime = 0;
}
int TrafficControlCenter::findNearestAvailableIntersection(int currentIntID) {
    int currentIdx = currentIntID - 1;
    int nearestInt = -1;
    for (int i = 0; i < numIntersections; i++) {
        if (adjacencyMatrix[currentIdx][i] > 0) {
            Intersection* targetInt = intersections[i];
            if (targetInt->isClosed()) {
                continue;
            }
            bool hasAvailableLane = false;
            for (int ln = 0; ln < targetInt->getLaneCount(); ln++) {
                if (!targetInt->isLaneBlocked(ln)) {
                    hasAvailableLane = true;
                    break;
                }
            }
            if (hasAvailableLane) {
                nearestInt = i + 1;
                break;
            }
        }
    }
    return nearestInt;
}
void TrafficControlCenter::rerouteVehiclesFromLane(int intID, int laneID) {
    int intIdx = intID - 1;
    Lane* sourceLane = intersections[intIdx]->getLane(laneID);
    if (!sourceLane) return;
    int targetIntID = findNearestAvailableIntersection(intID);
    if (targetIntID == -1) {
        return;
    }
    int targetIntIdx = targetIntID - 1;
    Queue<Vehicle*> vehiclesToMove;
    while (!sourceLane->isEmpty()) {
        Vehicle* v = sourceLane->dequeueFront();
        if (v) {
            vehiclesToMove.enqueue(v);
        }
    }
    while (!vehiclesToMove.isEmpty()) {
        Vehicle* v = vehiclesToMove.dequeue();
        v->setINT(targetIntID);
        v->setWT(v->getWT() + reroutingPenalty);
        int targetLaneID = findAvailableLane(targetIntID);
        if (targetLaneID != -1) {
            v->setLane(targetLaneID);
            Lane* targetLane = intersections[targetIntIdx]->getLane(targetLaneID);
            int priority = 0;
            if (v->getType() == EV) {
                priority = (v->getUrgency() * 2) - v->getWT();
            }
            targetLane->addVehicle(v, priority);
        }
    }
}

void TrafficControlCenter::rerouteVehiclesFromIntersection(int intID) {
    int intIdx = intID - 1;
    Intersection* inter = intersections[intIdx];
    for (int ln = 0; ln < inter->getLaneCount(); ln++) {
        rerouteVehiclesFromLane(intID, ln);
    }
}
int TrafficControlCenter::findAvailableLane(int intID) {
    int intIdx = intID - 1;
    Intersection* inter = intersections[intIdx];
    for (int ln = 0; ln < inter->getLaneCount(); ln++) {
        if (!inter->isLaneBlocked(ln)) {
            return ln;
        }
    }
    return 0;
}
void TrafficControlCenter::simulate(const char* outputFilename) {
    simulationMode = ui.getSimulationMode();
    ui.printSimulationStart();
    initialize();
    while (!eventQueue.isEmpty() || hasActiveVehicles() ||
        (numIntersections > 0 && intersections[0]->hasWaitingVehicles())) {
        if (!eventQueue.isEmpty()) {
            Event* event = eventQueue.peek();
            if (event->getTimestamp() > currentTime) {
                currentTime = event->getTimestamp();
            }
        }
        ui.printTimestep(currentTime);
        while (!eventQueue.isEmpty() && eventQueue.peek()->getType() == EVENT_CANCEL
            && eventQueue.peek()->getTimestamp() == currentTime) {
            Event* e = eventQueue.dequeue();
            handleCancellationEvent(e);
            ui.printEventHandled(e);
            delete e;
        }
        while (!eventQueue.isEmpty() && eventQueue.peek()->getType() == EVENT_PROMOTION
            && eventQueue.peek()->getTimestamp() == currentTime) {
            Event* e = eventQueue.dequeue();
            handlePromotionEvent(e);
            ui.printEventHandled(e);
            delete e;
        }
        while (!eventQueue.isEmpty() && (eventQueue.peek()->getType() == EVENT_ACCIDENT
            || eventQueue.peek()->getType() == EVENT_ROAD_CLOSURE)
            && eventQueue.peek()->getTimestamp() == currentTime) {
            Event* e = eventQueue.dequeue();
            if (e->getType() == EVENT_ACCIDENT) {
                handleAccidentEvent(e);
            }
            else if (e->getType() == EVENT_ROAD_CLOSURE) {
                handleRoadClosureEvent(e);
            }
            ui.printEventHandled(e);
            delete e;
        }
        while (!eventQueue.isEmpty() && eventQueue.peek()->getType() == EVENT_ARRIVAL
            && eventQueue.peek()->getTimestamp() == currentTime) {
            Event* e = eventQueue.dequeue();
            handleArrivalEvent(e);
            ui.printEventHandled(e);
            delete e;
        }
        decreaseAllBlockTimers();
        scheduleVehicles(currentTime);
        processCrossingVehicles(currentTime);
        checkAutoPromotion(currentTime);
        checkAutoCancellation(currentTime);
        incrementWaitingTimes(currentTime);
        for (int i = 0; i < numIntersections; i++) {
            if (intersections[i]->isClosed()) {
                intersectionBlockedTime[i]++;
            }
            else {
                bool hasBlockedLane = false;
                for (int ln = 0; ln < intersections[i]->getLaneCount(); ln++) {
                    if (intersections[i]->isLaneBlocked(ln)) {
                        hasBlockedLane = true;
                        break;
                    }
                }
                if (hasBlockedLane) {
                    intersectionBlockedTime[i]++;
                }
            }
        }
        ui.printQueuesState(intersections, numIntersections, currentTime);
        ui.waitForUser();
        currentTime++;
    }
    totalSimulationTime = currentTime;
    ui.printSimulationEnd();
    finalizeStatistics();
    writeOutputFile(outputFilename);
}
void TrafficControlCenter::scheduleVehicles(int currentTime) {
    for (int i = 0; i < numIntersections; i++) {
        Intersection* inter = intersections[i];
        if (inter->isClosed()) {
            continue;
        }
        inter->decreaseSwitchCooldown();
        int bestLane = findBestLaneToGreen(inter, currentTime);
        if (bestLane != -1) {
            int currentGreen = inter->getGreenLane();
            if (currentGreen != bestLane) {
                if (inter->getSwitchCooldown() == 0) {
                    inter->setGreenLane(bestLane);
                    recordSwitchAtIntersection(inter->getID());
                }
                else {
                    if (currentGreen != -1 && !inter->getLane(currentGreen)->isEmpty()) {
                        bestLane = currentGreen;
                    }
                }
            }
            else {
                inter->setGreenLane(bestLane);
            }
            selectAndStartCrossing(inter, bestLane, currentTime);
        }
    }
}
int TrafficControlCenter::findBestLaneToGreen(Intersection* inter, int currentTime) {
    int bestLane = -1;
    VehicleType bestType = NC;
    int bestPriority = -999999;
    for (int ln = 0; ln < inter->getLaneCount(); ln++) {
        Lane* lane = inter->getLane(ln);
        if (inter->isLaneBlocked(ln)) {
            continue;
        }
        if (lane->isEmpty()) {
            continue;
        }
        if (inter->getLaneLocked(ln)) {
            continue;
        }
        Vehicle* v = getHighestPriorityVehicle(lane, currentTime);
        if (v == nullptr) continue;

        VehicleType vType = v->getType();
        if (vType == EV) {
            if (bestType != EV) {
                bestLane = ln;
                bestType = EV;
                bestPriority = (v->getUrgency() * 2) - v->getWT();
            }
            else if (bestType == EV) {
                int vPriority = (v->getUrgency() * 2) - v->getWT();
                if (vPriority > bestPriority) {
                    bestLane = ln;
                    bestPriority = vPriority;
                }
            }
        }
        else if (vType == PT) {
            if (bestType != EV && bestType != PT) {
                bestLane = ln;
                bestType = PT;
            }
        }
        else if (vType == FV) {
            if (bestType == NC) {
                bestLane = ln;
                bestType = FV;
            }
        }
        else if (vType == NC) {
            if (bestType == NC && bestLane == -1) {
                bestLane = ln;
                bestType = NC;
            }
        }
    }

    return bestLane;
}
Vehicle* TrafficControlCenter::getHighestPriorityVehicle(Lane* lane, int currentTime) {
    return lane->peek();
}
void TrafficControlCenter::selectAndStartCrossing(Intersection* inter, int laneID, int currentTime) {
    if (laneID < 0 || laneID >= inter->getLaneCount()) {
        return;
    }
    Lane* lane = inter->getLane(laneID);
    if (inter->isLaneBlocked(laneID) || inter->isClosed()) {
        return;
    }
    if (lane->isEmpty()) {
        return;
    }
    if (inter->crossingVehicles[laneID] != nullptr) {
        return;
    }
    Vehicle* nextVehicle = lane->dequeueFront();
    if (nextVehicle == nullptr) {
        return;
    }
    inter->crossingVehicles[laneID] = nextVehicle;
    inter->remainingCrossTime[laneID] = nextVehicle->getXD();
    nextVehicle->setStatus(PASSING);
}
void TrafficControlCenter::recordSwitchAtIntersection(int intID) {
    if (intID >= 1 && intID <= numIntersections) {
        switchCountPerIntersection[intID - 1]++;
    }
}
void TrafficControlCenter::decreaseAllBlockTimers() {
    for (int i = 0; i < numIntersections; i++) {
        intersections[i]->decreaseBlockTimers();
        intersections[i]->decreaseClosureTimer();
    }
}
bool TrafficControlCenter::hasActiveVehicles() const {
    for (int i = 0; i < numIntersections; i++) {
        for (int ln = 0; ln < intersections[i]->getLaneCount(); ln++) {
            if (intersections[i]->crossingVehicles[ln] != nullptr) {
                return true;
            }
        }
    }
    return false;
}
void TrafficControlCenter::processCrossingVehicles(int currentTime) {
    for (int i = 0; i < numIntersections; i++) {
        for (int ln = 0; ln < intersections[i]->getLaneCount(); ln++) {
            Vehicle* crossing = intersections[i]->crossingVehicles[ln];
            if (crossing != nullptr) {
                intersections[i]->remainingCrossTime[ln]--;
                if (intersections[i]->remainingCrossTime[ln] <= 0) {
                    crossing->setStatus(FINISHED);
                    crossing->setCT(currentTime);
                    recordVehicleCompletion(crossing, currentTime);
                    intersections[i]->crossingVehicles[ln] = nullptr;
                    intersections[i]->remainingCrossTime[ln] = 0;
                }
            }
        }
    }
}

void TrafficControlCenter::handleArrivalEvent(Event* e) {
    if (!e || e->getType() != EVENT_ARRIVAL || !e->getVehicle()) return;
    Vehicle* v = e->getVehicle();
    int intIdx = v->getINT() - 1;
    Lane* lane = intersections[intIdx]->getLane(v->getLane());
    int priority = 0;
    if (v->getType() == EV) {
        priority = (v->getUrgency() * 2) - v->getWT();
    }
    lane->addVehicle(v, priority);
}
void TrafficControlCenter::handleCancellationEvent(Event* e) {
    if (!e || e->getType() != EVENT_CANCEL) return;
    Vehicle* v = findVehicle(e->getVehicleID());
    if (v) {
        int intIdx = v->getINT() - 1;
        Lane* lane = intersections[intIdx]->getLane(v->getLane());
        lane->removeVehicle(v);
        v->setStatus(CANCELED);
        recordVehicleCancellation(v);
    }
}
void TrafficControlCenter::handlePromotionEvent(Event* e) {
    if (!e || e->getType() != EVENT_PROMOTION) return;
    Vehicle* v = findVehicle(e->getVehicleID());
    if (v && v->getType() == PT) {
        int intIdx = v->getINT() - 1;
        Lane* lane = intersections[intIdx]->getLane(v->getLane());
        lane->removeVehicle(v);
        v->setType(EV);
        v->markPromoted();
        int priority = (v->getUrgency() * 2) - v->getWT();
        lane->addVehicle(v, priority);
        totalAutoPromoted++;
    }
}
void TrafficControlCenter::handleAccidentEvent(Event* e) {
    if (!e || e->getType() != EVENT_ACCIDENT) return;
    int intIdx = e->getIntersectionID() - 1;
    int laneID = e->getLaneID();
    int duration = e->getDuration();
    if (reroutingEnabled) {
        int vehicleCount = intersections[intIdx]->getLane(laneID)->totalVehicles();
        int targetIntID = findNearestAvailableIntersection(e->getIntersectionID());
        if (targetIntID != -1 && vehicleCount > 0) {
            ui.printRerouting(e->getIntersectionID(), targetIntID, vehicleCount);
        }
        rerouteVehiclesFromLane(e->getIntersectionID(), laneID);
    }
    ui.printAccidentBlocked(e->getIntersectionID(), laneID, duration);
    intersections[intIdx]->blockLane(laneID, duration);
}
void TrafficControlCenter::handleRoadClosureEvent(Event* e) {
    if (!e || e->getType() != EVENT_ROAD_CLOSURE) return;
    int intIdx = e->getIntersectionID() - 1;
    int duration = e->getDuration();
    if (reroutingEnabled) {
        int totalVehicles = intersections[intIdx]->getTotalWaitingVehicles();

        int targetIntID = findNearestAvailableIntersection(e->getIntersectionID());
        if (targetIntID != -1 && totalVehicles > 0) {
            ui.printRerouting(e->getIntersectionID(), targetIntID, totalVehicles);
        }
        rerouteVehiclesFromIntersection(e->getIntersectionID());
    }
    ui.printRoadClosure(e->getIntersectionID(), duration);
    intersections[intIdx]->closeIntersection(duration);
}
void TrafficControlCenter::checkAutoPromotion(int currentTime) {
    for (int i = 0; i < numIntersections; i++) {
        for (int j = 0; j < intersections[i]->getLaneCount(); j++) {
            Lane* lane = intersections[i]->getLane(j);
            if (!lane) continue;

            Node<Vehicle*>* cur = lane->getPTQueue().getFront();
            while (cur != nullptr) {
                Vehicle* v = cur->data;
                Node<Vehicle*>* next = cur->next;

                if (v && v->getStatus() == WAITING) {
                    int waitTime = currentTime - v->getAT();
                    v->setWT(waitTime);
                    if (waitTime >= v->getAP()) {
                        lane->removeVehicle(v);
                        v->setType(EV);
                        v->markPromoted();
                        int newPriority = (v->getUrgency() * 2) - waitTime;
                        lane->addVehicle(v, newPriority);
                        totalAutoPromoted++;
                    }
                }
                cur = next;
            }
        }
    }
}
void TrafficControlCenter::checkAutoCancellation(int currentTime) {
    for (int i = 0; i < numIntersections; i++) {
        for (int j = 0; j < intersections[i]->getLaneCount(); j++) {
            Lane* lane = intersections[i]->getLane(j);
            if (!lane) continue;

            Node<Vehicle*>* cur = lane->getNCQueue().getFront();
            while (cur != nullptr) {
                Vehicle* v = cur->data;
                Node<Vehicle*>* next = cur->next;
                if (v && v->getStatus() == WAITING) {
                    int waitTime = currentTime - v->getAT();
                    v->setWT(waitTime);
                    if (waitTime >= v->getCancelT()) {
                        lane->removeVehicle(v);
                        v->setStatus(CANCELED);
                        recordVehicleCancellation(v);
                    }
                }
                cur = next;
            }
        }
    }
}
void TrafficControlCenter::incrementWaitingTimes(int currentTime) {
    for (int i = 0; i < numIntersections; i++) {
        for (int j = 0; j < intersections[i]->getLaneCount(); j++) {
            Lane* lane = intersections[i]->getLane(j);
            if (lane) {
                lane->recalculateEVPriorities(currentTime);
            }
        }
    }
}
void TrafficControlCenter::recordVehicleCompletion(Vehicle* v, int completionTime) {
    if (!v) return;
    int wt = completionTime - v->getAT();
    v->setWT(wt);
    completedVehicles.enqueue(v);
    if (wt > maxWT) {
        maxWT = wt;
        mostDelayedVehicle = v;
    }
    VehicleType originalType = v->getType();
    if (v->isPromoted()) {
        totalEV++;
    }
    else {
        if (originalType == EV) totalEV++;
        else if (originalType == PT) totalPT++;
        else if (originalType == NC) totalNC++;
        else if (originalType == FV) totalFV++;
    }
}
void TrafficControlCenter::recordVehicleCancellation(Vehicle* v) {
    if (!v) return;
    int wt = currentTime - v->getAT();
    v->setWT(wt);
    canceledVehicles.enqueue(v);
    totalCanceled++;
    if (wt > maxWT) {
        maxWT = wt;
        mostDelayedVehicle = v;
    }
    VehicleType type = v->getType();
    if (type == EV) totalEV++;
    else if (type == PT) totalPT++;
    else if (type == NC) totalNC++;
    else if (type == FV) totalFV++;
}
void TrafficControlCenter::finalizeStatistics() {
    float sumWT_EV = 0, sumWT_PT = 0, sumWT_NC = 0, sumWT_FV = 0, sumWT_All = 0;
    float sumXD_EV = 0, sumXD_PT = 0, sumXD_NC = 0, sumXD_FV = 0, sumXD_All = 0;
    int countEV = 0, countPT = 0, countNC = 0, countFV = 0;
    Queue<Vehicle*> tempQueue;
    while (!completedVehicles.isEmpty()) {
        Vehicle* v = completedVehicles.dequeue();

        int wt = v->getWT();
        int xd = v->getXD();

        sumWT_All += wt;
        sumXD_All += xd;

        VehicleType originalType = v->getType();

        if (originalType == EV) {
            sumWT_EV += wt;
            sumXD_EV += xd;
            countEV++;
        }
        else if (originalType == PT) {
            sumWT_PT += wt;
            sumXD_PT += xd;
            countPT++;
        }
        else if (originalType == NC) {
            sumWT_NC += wt;
            sumXD_NC += xd;
            countNC++;
        }
        else if (originalType == FV) {
            sumWT_FV += wt;
            sumXD_FV += xd;
            countFV++;
        }

        tempQueue.enqueue(v);
    }
    if (totalSimulationTime > 0) {
        for (int i = 0; i < numIntersections; i++) {
            percentTimeBlocked[i] =
                ((float)intersectionBlockedTime[i] / totalSimulationTime) * 100.0f;
        }
    }
    while (!tempQueue.isEmpty()) {
        completedVehicles.enqueue(tempQueue.dequeue());
    }
    int totalCompleted = countEV + countPT + countNC + countFV;
    if (totalCompleted > 0) {
        avgWT_All = sumWT_All / totalCompleted;
        avgXD_All = sumXD_All / totalCompleted;
    }
    if (countEV > 0) {
        avgWT_EV = sumWT_EV / countEV;
        avgXD_EV = sumXD_EV / countEV;
    }
    if (countPT > 0) {
        avgWT_PT = sumWT_PT / countPT;
        avgXD_PT = sumXD_PT / countPT;
    }
    if (countNC > 0) {
        avgWT_NC = sumWT_NC / countNC;
        avgXD_NC = sumXD_NC / countNC;
    }
    if (countFV > 0) {
        avgWT_FV = sumWT_FV / countFV;
        avgXD_FV = sumXD_FV / countFV;
    }
    int totalVehicles = totalEV + totalPT + totalNC + totalFV;
    if (countPT > 0) {
        percentAutoPromoted = ((float)totalAutoPromoted / countPT) * 100.0f;
    }
    if (totalVehicles > 0) {
        percentCanceled = ((float)totalCanceled / totalVehicles) * 100.0f;
    }
}
void TrafficControlCenter::writeOutputFile(const char* filename) {
    ofstream out(filename);
    if (!out.is_open()) {
        return;
    }
    out << "CT ID AT WT XD TYPE INT LN\n";
    Queue<Vehicle*> temp;
    while (!completedVehicles.isEmpty()) {
        Vehicle* v = completedVehicles.dequeue();

        out << v->getCT() << " "
            << v->getID() << " "
            << v->getAT() << " "
            << v->getWT() << " "
            << v->getXD() << " "
            << v->getTypeName() << " "
            << v->getINT() << " "
            << v->getLane() << "\n";

        temp.enqueue(v);
    }

    while (!temp.isEmpty()) {
        completedVehicles.enqueue(temp.dequeue());
    }

    while (!canceledVehicles.isEmpty()) {
        Vehicle* v = canceledVehicles.dequeue();

        out << "-- "
            << v->getID() << " "
            << v->getAT() << " "
            << v->getWT() << " "
            << v->getXD() << " "
            << v->getTypeName() << " "
            << v->getINT() << " "
            << v->getLane() << " CANCELED\n";
    }

    out << "\n[STATISTICS]\n";
    out << "Total Vehicles: " << (totalEV + totalPT + totalNC + totalFV) << "\n";
    out << "Total EV: " << totalEV << " (including promotions)\n";
    out << "Total PT: " << totalPT << "\n";
    out << "Total NC: " << totalNC << "\n";
    out << "Total FV: " << totalFV << "\n\n";
    out << "Average WT (all): " << avgWT_All << "\n";
    out << "Average WT (EV): " << avgWT_EV << "\n";
    out << "Average WT (PT): " << avgWT_PT << "\n";
    out << "Average WT (NC): " << avgWT_NC << "\n";
    out << "Average WT (FV): " << avgWT_FV << "\n\n";
    out << "Average XD: " << avgXD_All << "\n\n";
    out << "% Auto-Promoted PT: " << percentAutoPromoted << "%\n";
    int totalSwitches = 0;
    for (int i = 0; i < numIntersections; i++) {
        totalSwitches += switchCountPerIntersection[i];
    }
    out << "Signal Switches: " << totalSwitches << "\n";

    out << "% Vehicles Canceled: " << percentCanceled << "%\n\n";

    out << "[ADDITIONAL STATISTICS]\n";
    if (mostDelayedVehicle) {
        out << "Most Delayed Vehicle: ID " << mostDelayedVehicle->getID()
            << " (WT = " << maxWT << " timesteps)\n";
    }
    else {
        out << "Most Delayed Vehicle: N/A\n";
    }
    out << "\n% Time Intersections Were Blocked (ACC/RC):\n";
    for (int i = 0; i < numIntersections; i++) {
        out << "  Intersection " << (i + 1) << ": "
            << percentTimeBlocked[i] << "% ("
            << intersectionBlockedTime[i] << "/"
            << totalSimulationTime << " timesteps)\n";
    }

    out.close();
}
Vehicle* TrafficControlCenter::findVehicle(int id) {
    for (int i = 0; i < numIntersections; i++) {
        for (int ln = 0; ln < intersections[i]->getLaneCount(); ln++) {
            Lane* lane = intersections[i]->getLane(ln);
            Vehicle* v = lane->findVehicleByID(id);
            if (v) return v;
        }
    }
    return nullptr;
}