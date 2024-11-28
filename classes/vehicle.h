#pragma once

#include "road.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "intersection.h"
using namespace std;

class Road;

struct Coord {
    int x, y;

    Coord(int i=0, int j=0): x(i), y(j) {}
};

class Vehicle {
    string name;
    vector<Road*> route;
    int currRoadIndex; // will change as road changes
    Coord position;
    Intersection* start, *end;

    public:
    Vehicle(string n) : name(n) {}
    Vehicle(string n, Intersection* a, Intersection* b) : name(n), start(a), end(b) {}

    void addPath(Road* r) {
        route.push_back(r);
    }
};

class EmergencyVehicle : public Vehicle { // dynamic routing
    string priority;

    public:
    EmergencyVehicle(string n, string p) : Vehicle(n), priority(p) {}
    EmergencyVehicle(string n, Intersection* a, Intersection* b , string p) : Vehicle(n, a, b), priority(p) {}
};

