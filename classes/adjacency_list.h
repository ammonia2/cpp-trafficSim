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
    Intersection* intersection;
    Road* road;
};

class AdjacencyList {
    vector<IntersectionRoad> roads; // temporary
    vector<LinkedList<Road*>> graph;
    vector<Intersection*> intersections;
    vector<Vehicle*> vehicles;
    vector<EmergencyVehicle*> emergencyVehicles;

    void loadEmergencyVehicles(fstream& file) {
        string line;
        int counter=0;
        while (getline(file, line)) {
            if (counter == 0) {
                counter += 1;
                continue;
            }
            stringstream ss(line);
            string veh, st, end, priority;
            getline(ss, veh, ',');
            getline(ss, st, ',');
            getline(ss, end, ',');
            getline(ss, priority, ',');

            EmergencyVehicle* vehicle = new EmergencyVehicle(veh, intersections[st[0]-'A'], intersections[end[0]-'A'], priority);
            emergencyVehicles.push_back(vehicle);
        }
        file.close();
    }

    void loadVehicles(fstream& file) {
        string line;
        int counter=0;
        while (getline(file, line)) {
            if (counter == 0) {
                counter += 1;
                continue;
            }
            stringstream ss(line);
            string veh, st, end;
            getline(ss, veh, ',');
            getline(ss, st, ',');
            getline(ss, end, ',');

            Vehicle* vehicle = new Vehicle(veh, intersections[st[0]-'A'], intersections[end[0]-'A']);
            vehicles.push_back(vehicle);
        }
        file.close();
    }

    void loadSignalTimings(fstream& file) {
        string line;
        int counter=0;
        while (getline(file, line)) {
            if (counter == 0) {
                counter += 1;
                continue;
            }
            string node, time;
            stringstream ss(line);
            getline(ss, node, ',');
            getline(ss, time, ',');

            intersections[node[0]-'A']->setGreenTime(stoi(time));
        }
        file.close();
    }

    void loadBlockages(fstream& file) { // for setting statuses of roads
        string line;
        int counter=0;
        while (getline(file, line)) {
            if (counter == 0) {
                counter += 1;
                continue;
            }
            stringstream ss(line);
            string start, end, status;
            getline(ss, start, ',');
            getline(ss,end , ',');
            getline(ss, status, ',');

            char startNode = start[0]; char endNode= end[0];
            LinkedList<Road*>& edges = graph[startNode-'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();

            Road* road=nullptr;
            while (edge) {
                if (edge->data->getDest()->getName() == endNode) {
                    road= edge->data;
                    road->setStatus(status);

                    break;
                }
                edge = edge->next;
            }

        }

        file.close();
    }

    void initialiseGraph() {
        fstream network_file("files/road_network.csv", ios::in);
        fstream signals_file("files/traffic_signal_timings.csv", ios::in);
        fstream vehicles_file("files/vehicles.csv", ios::in);
        fstream EmergencyVehicles_file("files/emergency_vehicles.csv", ios::in);
        fstream blockages_file("files/accidents_or_closures.csv", ios::in);

        string line;
        char maxNode='\0';
        int counter=0;
        while (getline(network_file, line)) {
            if (counter == 0) {
                counter += 1;
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
            maxNode = max(maxNode, max(startNode, endNode));
        
            // Ensure intersections vector is large enough
            while (intersections.size() <= (maxNode - 'A')) {
                char val = 'A' + intersections.size();
                Intersection* newNode = new Intersection(val);
                intersections.push_back(newNode);
            }
        
            Intersection* a = nullptr;
            for (Intersection* intsc : intersections) {
                if (intsc->getName() == startNode) {
                    a = intsc;
                    break;
                }
            }
        
            Intersection* b = nullptr;
            for (Intersection* intsc : intersections) {
                if (intsc->getName() == endNode) {
                    b = intsc;
                    break;
                }
            }
        
            if (a && b) {
                Road* edge = new Road(weightInt, b);
                roads.push_back({a, edge});
            } else {
                cout << "Intersection not found: " << startNode << " " << endNode << "\n";
            }
        }

        graph.resize(maxNode - 'A' + 1);

        // pushin the edges
        for (IntersectionRoad road: roads) {
            // separated Intersection and List for edge for ease of access.
            char startNode = road.intersection->getName();
            char endNode = road.road->getDest()->getName();
            graph[startNode - 'A'].insertAtEnd(road.road);

            intersections[startNode-'A']->addOutRoad(road.road);
            intersections[endNode -'A']->addInRoad(road.road);
        }

        network_file.close();
        loadBlockages(blockages_file);
        loadSignalTimings(signals_file);
        loadVehicles(vehicles_file);
        loadEmergencyVehicles(EmergencyVehicles_file);
    }

    public:
    AdjacencyList() {
        initialiseGraph();
    }
    
    void displayGraph(RenderWindow& window, int x,int y) {
        int i=0;
        for (Intersection* intsc : intersections) {
            char name = intsc->getName();
            cout<<name<<": ";
            graph[name-'A'].printList();
            // if(!graph[name- 'A'].isEmpty()){
            //     int index = 0;
            //     float angleStep=360/(graph[name- 'A'].getSize()+1);
            //     // float angleStep=90;
            //     float currentAngle=0.0;
            //     //Setting coordinates of Intersection
            //     if(intsc->get_X()==0 && intsc->get_Y()==0){
            //         // cout<<intsc->getName()<<endl;
            //         intsc->set_X(x);
            //         intsc->set_Y(y);
            //     }
            //     while (index<graph[name- 'A'].getSize()) {
            //         if((graph[name- 'A'].getNode(index))){
            //             Road* node_road = &(graph[name- 'A'].getNode(index)->data);
            //             // cout<<name<<" "<<intsc->get_X()<<" "<<intsc->get_Y()<<endl;;
            //             node_road->displayRoads(window, intsc->get_X()+20, intsc->get_Y(),currentAngle);
            //         }
            //         currentAngle+=angleStep;
            //         index++;
            //     }
            // }
            // intsc->displayIntersection(window,intsc->get_X(),intsc->get_Y());
            // break;
            // i+=1;
            // if(i==2)
            //     break;
            // x+=50;
            // y+=50;
            
        }
    }
};