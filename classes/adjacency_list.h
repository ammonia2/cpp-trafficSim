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

int stringToInt(const std::string& str) {
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
    std::vector<LinkedList<Road>> graph;
    std::vector<Intersection> intersections;
    std::vector<IntersectionRoad> roads;

    void initialiseGraph() {
        fstream network("files/road_network.csv", ios::in);
        fstream signals("files/traffic_signal_timings.csv", ios::in);

        std::string line;
        char maxNode;

        while (std::getline(network, line)) {
            stringstream ss(line);
            std::string start, end, weight;

            std::getline(ss, start, ',');
            std::getline(ss, end, ',');
            std::getline(ss, weight, ',');

            char startNode = start[0];
            char endNode = end[0];
            int weightInt = stringToInt(weight);

            Intersection a= Intersection(startNode);
            Intersection b= Intersection(endNode);

            Road edge = Road(weightInt, b);
            roads.push_back({a, edge});

            maxNode = std::max(maxNode, std::max(startNode, endNode));
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

        network.close();
        signals.close();
    }

    public:
    AdjacencyList() {
        initialiseGraph();
    }

    void displayGraph() {
        for (Intersection intsc : intersections) {
            char name = intsc.getName();
            cout<<name<<" goes to: ";
            graph[name- 'A'].display();
        }
    }
};