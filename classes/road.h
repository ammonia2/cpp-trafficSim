#pragma once

#include "vehicle.h"
#include "intersection.h"
#include <iostream>
#include "hashtable.h"
#include "priority_queue.h"
#include <cmath>
using namespace std;


class Vehicle;
class Intersection;

class Road {
    Intersection* dest;
    int travelTime; //weight
    PriorityQueue<Vehicle*> priority_Queue;
    string status;

    public:
    Road(int time, Intersection* b): dest(b), status("Clear"), priority_Queue(false) {
        travelTime = time;
    }

    void setStatus(string s) {
        status = s;
    }
    
    string getStatus(){
        return status;
    }

    Vehicle* getHeapTop() {
        return priority_Queue.top();
    }

    void clearQueue() {
        while (priority_Queue.getSize() > 0) {
            priority_Queue.pop();
        }
    }

    Intersection*& getDest() {
        return dest;
    }

    int getTrafficLoad() {
        return priority_Queue.getSize();
    }

    int getCapacity() const {
        return 1.0 / travelTime;
    }

    int getWeight() {
        return travelTime;
    }

    void insertVehicle(Vehicle* vehicle) {
        priority_Queue.push(vehicle);
    }

    void removeVehicle() {
        priority_Queue.pop();
    }

    void displayQ() {
        cout<<"YO\n";
        cout<<"Q size: "<<priority_Queue.getSize()<<endl;
        priority_Queue.display();
    }

    friend ostream& operator<<(ostream& os, const Road& road) {
        os << road.dest->getName();
        return os;
    }

};

