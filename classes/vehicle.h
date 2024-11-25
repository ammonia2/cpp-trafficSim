#pragma once

#include "road.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Road;

struct Coord {
    int x, y;

    Coord(int i=0, int j=0): x(i), y(j) {}
};

class Vehicle {
    std::string name;
    std::vector<Road*> route;
    int currRoadIndex; // will change as road changes
    Coord position;

    public:
    Vehicle(std::string n) : name(n) {}

    void addPath(Road* r) {
        route.push_back(r);
    }
};

class EmergencyVehicle : public Vehicle { // dynamic routing
    std::string priority;

    public:
    EmergencyVehicle(std::string n, std::string p) : Vehicle(n), priority(p) {}
};

