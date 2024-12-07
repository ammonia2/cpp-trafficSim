#pragma once

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

    void setYellowTime(int y) {
        yellow = y;
    }

    void setCurrentTime(int c) {
        currentTime = c;
    }

    int getRedTime() {
        return red;
    }

    void update() {
        if (currentState == "GREEN") {
            if (currentTime>0) currentTime--;
            if (currentTime <= 0 && yellow != 0) {
                currentState = "YELLOW";
                currentTime = yellow;
            }
            else if (currentTime <= 0) {
                currentTime = green;
            }
        } else if (currentState == "YELLOW") {
            if (currentTime>0) currentTime--;
            if (currentTime <= 0) {
                currentState = "RED";
                currentTime = red;
            }
        } else if (currentState == "RED") {
            if (currentTime>0) currentTime--;
            if (currentTime <= 0) {
                currentState = "GREEN";
                currentTime = green;
            }
        }
    }

    void setState(string s) {
        currentState =s;
    }

    void resetTime() {
        if (currentState == "GREEN") {
            currentTime = green;
        } else if (currentState == "YELLOW") {
            currentTime = yellow;
        } else if (currentState == "RED") {
            currentTime = red;
        }
    }

    int getGreenTime() const {
        return green;
    }

    string getState() const {
        return currentState;
    }

    int getTimeRemaining() const {
        return currentTime;
    }
};

