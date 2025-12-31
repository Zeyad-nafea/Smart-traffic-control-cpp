#include "Lanes.h"
#include "Vehicle.h"



Lane::Lane() {
    intersectionID = 0;
    laneID = 0;
    blocked = false;
    remainingBlockedDuration = 0;
    laneLocked = false;
}

Lane::Lane(int intID, int lID) {
    intersectionID = intID;
    laneID = lID;
    blocked = false;
    remainingBlockedDuration = 0;
    laneLocked = false;
}

Lane::~Lane() {

}



int Lane::getIntersectionID() const {
    return intersectionID;
}

int Lane::getLaneID() const {
    return laneID;
}

bool Lane::isBlocked() const {
    return blocked;
}

bool Lane::isLaneLocked() const {
    return laneLocked;
}



bool Lane::canEnterLane() const {
    return !blocked && !laneLocked;
}

void Lane::blockLane(int duration) {
    blocked = true;
    remainingBlockedDuration = duration;
}

void Lane::unblockLane() {
    blocked = false;
    remainingBlockedDuration = 0;
}

void Lane::updateBlockedLane() {
    if (blocked) {
        remainingBlockedDuration--;
        if (remainingBlockedDuration <= 0) {
            unblockLane();
        }
    }
}



void Lane::lockLane() {
    laneLocked = true;
}

void Lane::unlockLane() {
    laneLocked = false;
}



void Lane::addVehicle(Vehicle* v, int priority) {
    if (!v) return;


    if (!canEnterLane()) {
        return;
    }

    VehicleType type = v->getType();


    if (type == EV) {
        EV_queue.enqueue(v, priority);
    }
    else if (type == PT) {
        PT_queue.enqueue(v);
    }
    else if (type == NC) {
        NC_queue.enqueue(v);
    }
    else if (type == FV) {
        FV_queue.enqueue(v);
    }
}



Vehicle* Lane::peek() const {

    if (!EV_queue.isEmpty()) return EV_queue.peek();
    if (!PT_queue.isEmpty()) return PT_queue.peek();
    if (!NC_queue.isEmpty()) return NC_queue.peek();
    if (!FV_queue.isEmpty()) return FV_queue.peek();
    return nullptr;
}



Vehicle* Lane::dequeueFront() {

    if (!EV_queue.isEmpty()) return EV_queue.dequeue();
    if (!PT_queue.isEmpty()) return PT_queue.dequeue();
    if (!NC_queue.isEmpty()) return NC_queue.dequeue();
    if (!FV_queue.isEmpty()) return FV_queue.dequeue();
    return nullptr;
}


bool Lane::isEmpty() const {
    return EV_queue.isEmpty() &&
        PT_queue.isEmpty() &&
        NC_queue.isEmpty() &&
        FV_queue.isEmpty();
}


int Lane::totalVehicles() const {
    return EV_queue.size() +
        PT_queue.size() +
        NC_queue.size() +
        FV_queue.size();
}



Vehicle* Lane::findVehicleByID(int id) const {
    Node<Vehicle*>* cur = nullptr;


    cur = EV_queue.getFront();
    while (cur) {
        if (cur->data && cur->data->getID() == id) {
            return cur->data;
        }
        cur = cur->next;
    }


    cur = PT_queue.getFront();
    while (cur) {
        if (cur->data && cur->data->getID() == id) {
            return cur->data;
        }
        cur = cur->next;
    }


    cur = NC_queue.getFront();
    while (cur) {
        if (cur->data && cur->data->getID() == id) {
            return cur->data;
        }
        cur = cur->next;
    }


    cur = FV_queue.getFront();
    while (cur) {
        if (cur->data && cur->data->getID() == id) {
            return cur->data;
        }
        cur = cur->next;
    }

    return nullptr;
}



bool Lane::removeVehicle(Vehicle* v) {
    if (!v) return false;

    VehicleType type = v->getType();


    if (type == EV) {
        return EV_queue.delitem(v);
    }
    else if (type == PT) {
        return PT_queue.delitem(v);
    }
    else if (type == NC) {
        return NC_queue.delitem(v);
    }
    else if (type == FV) {
        return FV_queue.delitem(v);
    }
    else {
        return false;
    }
}



void Lane::updateAllWaitingTimes(int currentTime) {
    Node<Vehicle*>* cur = nullptr;


    cur = EV_queue.getFront();
    while (cur != nullptr) {
        if (cur->data) {
            int newWT = currentTime - cur->data->getAT();
            cur->data->setWT(newWT);
        }
        cur = cur->next;
    }


    cur = PT_queue.getFront();
    while (cur != nullptr) {
        if (cur->data) {
            int newWT = currentTime - cur->data->getAT();
            cur->data->setWT(newWT);
        }
        cur = cur->next;
    }


    cur = NC_queue.getFront();
    while (cur != nullptr) {
        if (cur->data) {
            int newWT = currentTime - cur->data->getAT();
            cur->data->setWT(newWT);
        }
        cur = cur->next;
    }


    cur = FV_queue.getFront();
    while (cur != nullptr) {
        if (cur->data) {
            int newWT = currentTime - cur->data->getAT();
            cur->data->setWT(newWT);
        }
        cur = cur->next;
    }
}


void Lane::recalculateEVPriorities(int currentTime) {
    if (EV_queue.isEmpty()) return;


    Queue<Vehicle*> tempQueue;


    while (!EV_queue.isEmpty()) {
        Vehicle* v = EV_queue.dequeue();
        if (v) {

            int newWT = currentTime - v->getAT();
            v->setWT(newWT);


            tempQueue.enqueue(v);
        }
    }


    while (!tempQueue.isEmpty()) {
        Vehicle* v = tempQueue.dequeue();
        if (v) {

            int newPriority = (v->getUrgency() * 2) - v->getWT();
            EV_queue.enqueue(v, newPriority);
        }
    }
}



const PriorityQueue<Vehicle*>& Lane::getEVQueue() const {
    return EV_queue;
}

const Queue<Vehicle*>& Lane::getPTQueue() const {
    return PT_queue;
}

const Queue<Vehicle*>& Lane::getNCQueue() const {
    return NC_queue;
}

const Queue<Vehicle*>& Lane::getFVQueue() const {
    return FV_queue;
}



int Lane::getQueueSize(VehicleType type) const {
    if (type == EV) {
        return EV_queue.size();
    }
    else if (type == PT) {
        return PT_queue.size();
    }
    else if (type == NC) {
        return NC_queue.size();
    }
    else if (type == FV) {
        return FV_queue.size();
    }
    else {
        return 0;
    }
}



void Lane::printLaneState() const {

}