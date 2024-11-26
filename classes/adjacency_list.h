#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include "road.h"
#include "intersection.h"
#include "vehicle.h"
#include "traffic_signal.h"
#include "list.h"
using namespace std;
using namespace sf;

int stringToInt(const string& str) {
    int result = 0;

    for (int i = 0; i < str.length(); ++i) {
        char c = str[i];
        int digit = c - '0';
        result = result * 10 + digit;
    }

    return result;
}

struct IntersectionRoad {
    Intersection intersection;
    Road road;
};

class AdjacencyList {
    vector<LinkedList<Road>> graph;
    vector<Intersection> intersections;
    vector<IntersectionRoad> roads;

    void initialiseGraph() {
        fstream network("files/road_network.csv", ios::in);
        fstream signals("files/traffic_signal_timings.csv", ios::in);

        string line;
        char maxNode='\0';
        int counter=0;
        while (getline(network, line)) {
            if(counter==0){
                counter+=1;
                continue;
            }
            stringstream ss(line);
            string start, end, weight;

            getline(ss, start, ',');
            getline(ss, end, ',');
            getline(ss, weight, ',');

            char startNode = start[0];
            char endNode = end[0];
            int weightInt = stringToInt(weight);

            Intersection a= Intersection(startNode);
            Intersection b= Intersection(endNode);

            Road edge = Road(weightInt, b);
            roads.push_back({a, edge});

            maxNode = max(maxNode, max(startNode, endNode));
        }
        // creating the array to store edges for intersections
        for (char ch = 'A'; ch<=maxNode; ch++) {
            intersections.push_back(ch);
            LinkedList<Road> list;
            graph.push_back(list);
        }

        // pushin the edges
        for (IntersectionRoad road: roads) {
            // separated Intersection and List for edge for ease of access.
            char startNode = road.intersection.getName();
            char endNode = road.road.getDest().getName();
            graph[startNode - 'A'].insertAtEnd(road.road);
            Road* roadPointer = &road.road;
            intersections[startNode-'A'].addOutRoad(roadPointer);
            intersections[endNode -'A'].addInRoad(roadPointer);
        }
        // for (int i = 0; i < graph.size() - 1; ++i) {
        //     for (int j = 0; j < graph.size() - i - 1; ++j) {
        //         int a=0,b=0;
        //         if (!graph[j].isEmpty()) {
        //             // cout<<"yes"<<endl;
        //             a=graph[j].getSize();
        //         }
        //         if(!graph[j+1].isEmpty()){
        //             // cout<<"yes1"<<endl;
        //             b=graph[j + 1].getSize();
        //         }
        //         if (a > b) {
        //             swap(graph[j], graph[j + 1]);
        //         }
        //     }
        // }
        // std::sort(graph.begin(), graph.end(), [](const LinkedList<Road>& a, const LinkedList<Road>& b) {
        // return a.getSize() > b.getSize(); // Descending order
        // });

        network.close();
        signals.close();
    }

    public:
    AdjacencyList() {
        initialiseGraph();
    }
    
    void displayGraph(RenderWindow& window, int x,int y) {
        int i=0;
        for (Intersection intsc : intersections) {
            char name = intsc.getName();
            if(!graph[name- 'A'].isEmpty()){
                int index = 0;
                float angleStep=360/(graph[name- 'A'].getSize()+1);

                //Setting coordinates of Intersection
                if(intsc.get_X()==0 && intsc.get_Y()==0){
                    cout<<intsc.getName()<<endl;
                    intsc.set_X(x);
                    intsc.set_Y(y);
                }
                float currentAngle=0.0;
                while (index<graph[name- 'A'].getSize()) {
                    if((graph[name- 'A'].getNode(index))){
                        Road* node_road = &(graph[name- 'A'].getNode(index)->data);
                        node_road->displayRoads(window, intsc.get_X()+20, intsc.get_Y(),currentAngle);
                    }
                    currentAngle+=angleStep;
                    index++;
                }
            }
            intsc.displayIntersection(window,intsc.get_X(),intsc.get_Y());
            i+=1;
            if(i==2)
                break;
            // x+=50;
            // y+=50;
            
        }
    }
};