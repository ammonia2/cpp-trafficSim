#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "road.h"
#include "traffic_signal.h"
using namespace std;

class Road;

class Intersection {
    char name;
    vector<Road*>inRoads;
    vector<Road*>outRoads;
    vector<TrafficSignal> signals;
    
    public:
    Intersection (char n) {
        name =n;
    }

    void addOutRoad(Road* r) {
        outRoads.push_back(r);
    }

    void addInRoad(Road* r) {
        inRoads.push_back(r);
    }

    char getName() const {
        return name;
    }
};

 