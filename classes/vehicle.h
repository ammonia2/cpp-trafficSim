#pragma once

#include "road.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "vector.h"
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

    public:
    Vehicle(string n) : name(n), currRoadIndex(0), atDest(false) {}

    Vehicle(string n, Intersection* a, Intersection* b) : name(n), start(a), end(b), currRoadIndex(0), atDest(false) {}

    void addRoad(Road* r) {
        route.push_back(r);
    }

    void setPriorityLevel(int p) {
        priorityLevel = p;
    }

    string getName() {
        return name;
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
    
    void moveIndex() {
        currRoadIndex+=1;
    }

    void setAtDest(bool arg) {
        atDest = true;
    }

    bool getAtDest() {
        return atDest;
    }

    int getPriorityLevel() {
        return priorityLevel;
    }

    bool operator<(const Vehicle& other) const {
        cout<<"<<<<<<<<<<<<<<<Miseter > called>>>>>>>>>>>>>>>\n";
        return this->priorityLevel < other.priorityLevel;
    }

    bool operator>(const Vehicle& other) const {
        cout<<"<<<<<<<<<<<<<<<Miseter > called>>>>>>>>>>>>>>>\n";
        return this->priorityLevel > other.priorityLevel;
    }

    bool operator<=(const Vehicle& other) const {
        return this->priorityLevel <= other.priorityLevel;
    }

    bool operator>=(const Vehicle& other) const {
        return this->priorityLevel >= other.priorityLevel;
    }

    bool operator==(const Vehicle& other) const {
        return this->priorityLevel == other.priorityLevel;
    }

    virtual ~Vehicle() {} // Virtual destructor for polymorphism
};

class EmergencyVehicle : public Vehicle { // dynamic routing
    string priority;

    public:
    EmergencyVehicle(string n, string p) : Vehicle(n), priority(p) {
        setPriorityLevel();
    }

    EmergencyVehicle(string n, Intersection* a, Intersection* b , string p) : Vehicle(n, a, b), priority(p) {
        setPriorityLevel();
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