#pragma once

#include <SFML/Graphics.hpp>
#include "vehicle.h"
#include "intersection.h"
#include <iostream>
#include <vector>

class Vehicle;

class Road {
    Intersection dest;
    int travelTime; //weight
    std::vector<Vehicle*> vehicles;

    public:
    Road(int time, Intersection& b): dest(b) {
        travelTime = time;
    }

    Intersection& getDest() {
        return dest;
    }

    friend std::ostream& operator<<(std::ostream& os, const Road& road) {
        os << road.dest.getName();
        return os;
    }
};

