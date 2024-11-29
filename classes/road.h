#pragma once

#include <SFML/Graphics.hpp>
#include "vehicle.h"
#include "intersection.h"
#include <iostream>
#include "vector.h"
#include <cmath>
using namespace std;
using namespace sf;


class Vehicle;

class Road {
    Intersection* dest;
    int travelTime; //weight
    Vector<Vehicle*> vehicles;
    string status;

    public:
    Road(int time, Intersection* b): dest(b) {
        travelTime = time;
    }

    void setStatus(string s) {
        status = s;
    }

    Intersection*& getDest() {
        return dest;
    }

    int getWeight() {
        return travelTime;
    }

    friend ostream& operator<<(ostream& os, const Road& road) {
        os << road.dest->getName();
        return os;
    }
    void displayRoads(RenderWindow& window ,int x,int y,float angle){
        int direc=1;
        if(angle>=180)
            direc=-1;

        //Setting coordinates of Intersection
        if(dest->get_X()==0 && dest->get_Y()==0){
            float radianAngle = angle * 3.14159265359 / 180.0;
            int length = travelTime;
            int xEnd = x + length * cos(radianAngle);
            int yEnd = y + length * sin(radianAngle);
            dest->set_X(xEnd);
            dest->set_Y(yEnd);
        }
        cout<<dest->getName()<<" "<<dest->get_X()<<" "<<dest->get_Y()<<endl;
        ConvexShape rectangle;
        rectangle.setPointCount(4);
        rectangle.setPoint(0, Vector2f(x,y+5));  // Top-left
        rectangle.setPoint(1, Vector2f(dest->get_X(),dest->get_Y()+5));  // Top-right
        rectangle.setPoint(2, Vector2f(dest->get_X(),dest->get_Y()+10));  // Bottom-right
        rectangle.setPoint(3, Vector2f(x, y+10));  // Bottom-left
        rectangle.setFillColor(Color::White);

        window.draw(rectangle);

        dest->displayIntersection(window,dest->get_X(),dest->get_Y());
    }
};

