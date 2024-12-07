#pragma once

#include <iostream>
#include "DynamicArray.h"
#include "traffic_signal.h"
using namespace std;

class Road;

class Intersection {
    char name;
    DynamicArray<Road*>inRoads;
    DynamicArray<Road*>outRoads;
    DynamicArray<TrafficSignal> signals;
    int greenTime;
    int x,y;

    public:
    Intersection (char n): x(0), y(0) {
        name =n;
    }

    void setGreenTime(int t) {
        greenTime = t;
    }

    void addOutRoad(Road* r);

    void addInRoad(Road* r);

    void set_X(int a){
        x=a;
    }

    void set_Y(int a){
        y=a;
    }
    
    DynamicArray<Road*> getInroads(){
        return inRoads;
    }

    string getSignal(Road* road);
    
    int get_X(){
        return x;
    }

    int get_Y(){
        return y;
    }
    char getName() const {
        return name;
    }
    int getSignalTime(Road* road){
        for (int i=0; i<inRoads.size(); i++ ) {
            if (inRoads[i] == road) {
                return signals[i].getTimeRemaining();
            }
        }
        return -1;
    }
    void calculateSignalTimings();

    void updateSignals();

    bool signalActive(Road* road) {
        for (int i=0; i<inRoads.size(); i++ ) {
            if (inRoads[i] == road) {
                return signals[i].getState() == "GREEN";
            }
        }
        return false;
    }

};

 