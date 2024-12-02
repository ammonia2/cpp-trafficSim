#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class TrafficSignal {
    int green, yellow, red;
    string currentState;
    int currentTime;

    public:
    TrafficSignal(int g=0, int r =69) {
        green = g;
        yellow = 3;
        red =r;
    }

    void setGreenTime(int g) {
        green = g;
    }

    void setRedTime(int r) {
        red = r;
    }

    int getRedTime() {
        return red;
    }

    void update() {
        if (currentState == "GREEN") {
            currentTime--;
            if (currentTime <= 0) {
                currentState = "YELLOW";
                currentTime = yellow;
            }
        } else if (currentState == "YELLOW") {
            currentTime--;
            if (currentTime <= 0) {
                currentState = "RED";
                currentTime = red;
            }
        } else if (currentState == "RED") {
            currentTime--;
            if (currentTime <= 0) {
                currentState = "GREEN";
                currentTime = green;
            }
        }
    }

    void setState(string s) {
        currentState =s;
    }

    void resetTime() {}

    string getState() const {
        return currentState;
    }

    int getTimeRemaining() const {
        return currentTime;
    }
};

