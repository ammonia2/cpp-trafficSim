#pragma once

#include <iostream>
#include "vector.h"
#include <fstream>
#include <string>
#include <sstream>

#include "road.h"
#include "intersection.h"
#include "vehicle.h"
#include "traffic_signal.h"
#include "list.h"
#include "priority_queue.h"

using namespace std;
using namespace sf;

#define INT_MAX 10000000

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

class DistanceNode { // for priority q
public:
    Intersection* intersection;
    int distance;

    DistanceNode() {}

    DistanceNode(Intersection* intersection, int distance) : intersection(intersection), distance(distance) {}

    bool operator<(const DistanceNode& other) const {
        return distance < other.distance;
    }

    bool operator > (const DistanceNode& other) const {
        return distance > other.distance;
    }
};

class AdjacencyList {
    Vector<IntersectionRoad> roads; // temporary
    Vector<LinkedList<Road*>> graph;
    Vector<Intersection*> intersections;
    Vector<Vehicle*> vehicles;
    Vector<EmergencyVehicle*> emergencyVehicles;

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

    void initialiseVehicles() {
        for (Vehicle* vehicle: vehicles) {
            Vector<char> refs = dijkstraAlgo(vehicle->getStart(), vehicle->getEnd());
            Vector<char> path = constructPath(refs, vehicle->getStart(), vehicle->getEnd());
            path.display();
            for (int i=0; i<path.size()-1; i++) {
                char c = path[i];
                char cNext= path[i+1];
                Road* road = nullptr;
                LinkedList<Road*>& edges = graph[c-'A'];
                LinkedList<Road*>::Node* edge = edges.getHead();

                while (edge) {
                    if (edge->data->getDest() -> getName() == cNext) {
                        road= edge->data;
                        break;
                    }
                    edge = edge->next;
                }

                if (road)
                    vehicle->addRoad(road);
            }
        }
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
        
            // Ensure intersections Vector is large enough
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
        initialiseVehicles();
    }

    public:
    AdjacencyList() {
        initialiseGraph();
    }
    
    Vector<char> dijkstraAlgo(Intersection* start, Intersection* end) { // returns a list of closest previous nodes for each nodes that can be tracked to a single starting point through constructPath
        // setting initial distances to MAX, visited, and priority q
        Vector<int> distance;
        Vector<char> previousReferences;
        Vector<bool> visited;
        PriorityQueue<DistanceNode> pq(true, intersections.size());
    
        distance.resize(intersections.size());
        previousReferences.resize(intersections.size());
        visited.resize(intersections.size());
        for (bool v: visited) v=false;
    
        for (Intersection* intsc : intersections) {
            distance[intsc->getName() - 'A'] = INT_MAX;
            previousReferences[intsc->getName() - 'A'] = '\0';
        }
    
        distance[start->getName() - 'A'] = 0;
        pq.push(DistanceNode(start, 0));
    
        // main algo
        while (!pq.isEmpty()) {
            DistanceNode minNode = pq.top();
            pq.pop();
    
            Intersection* minIntersection = minNode.intersection;
            char minNodeName = minIntersection->getName();
    
            if (visited[minNodeName - 'A']) {
                continue;
            }
    
            visited[minNodeName - 'A'] = true;
    
            LinkedList<Road*>& edges = graph[minNodeName - 'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();
    
            while (edge) {
                Intersection* neighbour = edge->data->getDest();
                int weight = edge->data->getWeight();
                int tempDistance = distance[minNodeName - 'A'] + weight;
    
                if (tempDistance < distance[neighbour->getName() - 'A']) {
                    distance[neighbour->getName() - 'A'] = tempDistance;
                    previousReferences[neighbour->getName() - 'A'] = minNodeName;
                    pq.push(DistanceNode(neighbour, tempDistance));
                }
    
                edge = edge->next;
            }
        }
    
        return previousReferences;
    }
    
    Vector<char> constructPath(Vector<char>& previousRefs, Intersection* start, Intersection* end) {
        Vector<char> path;
        for (char at = end->getName(); at != '\0'; at = previousRefs[at - 'A']) {
            path.push_back(at);
        }
        reverse(path.begin(), path.end());
    
        if (path.empty() || path[0] != start->getName()) {
            path.clear();
        }
    
        return path;
    }

    void aStarAlgo() {}

    void updateSimulation() {
        cout<<"meoew\n";
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
