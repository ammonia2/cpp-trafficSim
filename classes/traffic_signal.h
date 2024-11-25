#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
using namespace std;

class TrafficSignal {
    int green, yellow;
    enum state {RED, YELLOW, GREEN};

    public:
    TrafficSignal(int g=0) {
        green = g;
        yellow = 3;
    }
};

