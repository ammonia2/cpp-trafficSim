#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "vector.h"
#include "road.h"
#include "traffic_signal.h"
using namespace std;
using namespace sf;

class Road;

class Intersection {
    char name;
    Vector<Road*>inRoads;
    Vector<Road*>outRoads;
    Vector<TrafficSignal> signals;
    int greenTime;
    int x,y;

    public:
    Intersection (char n):x(0),y(0) {
        name =n;
    }

    void setGreenTime(int t) {
        greenTime = t;
    }
    void addOutRoad(Road* r) {
        outRoads.push_back(r);
    }
    void set_X(int a){
        x=a;
    }
    void set_Y(int a){
        y=a;
    }
    int get_X(){
        return x;
    }
    int get_Y(){
        return y;
    }
    void addInRoad(Road* r) {
        inRoads.push_back(r);
    }

    char getName() const {
        return name;
    }
    void displayIntersection(RenderWindow& window,int x, int y){
        CircleShape circle(15);
        circle.setFillColor(Color::Green);
        circle.setPosition(x-7, y+3);
        Font font;
        font.loadFromFile("files/Typographica-Blp5.ttf");
        Text text(name, font,25);
        text.setFillColor(Color::Black);
        text.setPosition(x,y);
        window.draw(circle);
        window.draw(text);
        
    }
};

 