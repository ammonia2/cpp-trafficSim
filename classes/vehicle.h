#pragma once

#include "road.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "intersection.h"
using namespace std;

class Road;

struct Coord {
    int x, y;

    Coord(int i=0, int j=0): x(i), y(j) {}
};

class Vehicle {
    protected:
    string name;
    Vector<Road*> route;
    int currRoadIndex; // will change as road changes
    Coord position;
    Intersection* start, *end;
    bool atDest;
    int priorityLevel;
    int travelTime;

    public:
    Vehicle(string n, int p=0) : name(n), currRoadIndex(0), atDest(false), priorityLevel(p) {}

    Vehicle(string n, Intersection* a, Intersection* b, int p=0) : name(n), start(a), end(b), currRoadIndex(0), atDest(false), priorityLevel(p) {}

    void setRoad();

    void addRoad(Road* r) {
        route.push_back(r);
        // cout<<"New routing: ";r->displayQ();
        // cout<<endl;
    }

    void setPriorityLevel(int p) {
        priorityLevel = p;
    }

    string getName() {
        return name;
    }

    int getTime() {
        return travelTime;
    }

    void setTime(int t) {
        travelTime= t;
    }

    void updateTime() {
        if (travelTime>0) travelTime--;
    }

    Intersection* getStart() {
        return start;
    }

    Intersection* getEnd() {
        return end;
    }

    Vector<Road*> getRoute() {
        return route;
    }

    int getIndex(){
        return currRoadIndex;
    }

    void clearRoute() {
        route.clear();
    }

    void setIndex(int i) {
        currRoadIndex = i;
    }
    
    void moveIndex();

    void setAtDest(bool arg) {
        atDest = true;
    }

    bool getAtDest() {
        return atDest;
    }

    int getPriorityLevel() {
        return priorityLevel;
    }

    virtual void setOld(char o) {}

    virtual char getOld() {}

    bool operator<(const Vehicle& other) const {
        if (this->travelTime == other.travelTime) {
            return this->priorityLevel < other.priorityLevel;
        }
        return this->travelTime > other.travelTime;
    }

    bool operator>(const Vehicle& other) const {
        if (this->travelTime == other.travelTime) {
            return this->priorityLevel > other.priorityLevel;
        }
        return this->travelTime < other.travelTime;
    }

    bool operator<=(const Vehicle& other) const {
        return !(*this > other);
    }

    bool operator>=(const Vehicle& other) const {
        return !(*this < other);
    }

    bool operator==(const Vehicle& other) const {
        return this->priorityLevel == other.priorityLevel &&
            this->travelTime == other.travelTime;
    }

    friend ostream& operator<< (ostream& os, Vehicle* obj) {
        os<<obj->getName()<<" ";
        return os;
    }

    virtual ~Vehicle() {}
};

class EmergencyVehicle : public Vehicle { // dynamic routing
    string priority;
    char old;

    public:
    EmergencyVehicle(string n, string p) : Vehicle(n), priority(p) {
        setPriorityLevel();
    }

    EmergencyVehicle(string n, Intersection* a, Intersection* b , string p) : Vehicle(n, a, b), priority(p) {
        setPriorityLevel();
    }

    void setOld(char o) override {
        old = o;
    }
    char getOld() override {
        return old;
    }

    void setPriorityLevel() {
        if (priority=="High")
            priorityLevel = 69;
        else if (priority=="Medium")
            priorityLevel = 20;
        else if (priority=="Low")
            priorityLevel = 10;
    }
};