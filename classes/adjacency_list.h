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
#include "hashtable.h"

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

    friend ostream& operator << (ostream& ss, const DistanceNode& n) {
        ss<<n.intersection->getName()<<" ";
        return ss;
    }
};

class AdjacencyList {
    Vector<IntersectionRoad> roads; // temporary
    Vector<LinkedList<Road*>> graph;
    Vector<Intersection*> intersections;
    Vector<Vehicle*> vehicles;
    Vector<EmergencyVehicle*> emergencyVehicles;
    HashTable<string, LinkedList<Vehicle*>*> roadVehicleMap;

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

            if (!priority.empty() && priority.back() == '\r') {
                priority.pop_back();
            }

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

            if (!time.empty() && time.back() == '\r') {
                time.pop_back();
            }

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

            if (!status.empty() && status.back() == '\r') {
                status.pop_back();
            }

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

    void initialiseEmergencyVehicles() {}

    void initialiseVehicles() {
        for (Vehicle* vehicle: vehicles) {
            Vector<char> refs = dijkstraAlgo(vehicle->getStart(), vehicle->getEnd());
            Vector<char> path = constructPath(refs, vehicle->getStart(), vehicle->getEnd());
            
            if(!path.empty()) {
                cout<<path.front()<<" to "<<path.back()<<" : ";
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

                    if (road){
                        vehicle->addRoad(road);
                    }
                }

                //Initializing HashMap of vehicles on roads
                string key="";
                key+=path[0];
                key+=path[1];
                if (roadVehicleMap.find(key)) {
                    roadVehicleMap.search(key)->insertAtStart(vehicle);
                } 
                else {
                    LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                    newList->insertAtStart(vehicle);
                    roadVehicleMap.insert(key, newList);
                }
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

        // initialise signal timings
        for (Intersection* intsc: intersections) {
            intsc->calculateSignalTimings();
        }
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
    
            if (visited[minNodeName - 'A'])
                continue;
            
            visited[minNodeName - 'A'] = true;
    
            LinkedList<Road*>& edges = graph[minNodeName - 'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();

            while (edge) {
                if(edge->data->getStatus() == "Clear") {
                    Intersection* neighbour = edge->data->getDest();
                    int weight = edge->data->getWeight();
                    int tempDistance = distance[minNodeName - 'A'] + weight;
        
                    if (tempDistance < distance[neighbour->getName() - 'A']) {
                        distance[neighbour->getName() - 'A'] = tempDistance;
                        previousReferences[neighbour->getName() - 'A'] = minNodeName;
                        pq.push(DistanceNode(neighbour, tempDistance));
                    }
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
            if(path.empty()){
                cout<<start->getName()<<" to "<<end->getName()<<" : Path not Found"<<endl;
            }
        }
        return path;
    }

    void aStarAlgo() { // for emergency vehicle routing
    
    }

    void updateSimulation() {
        for(Vehicle* veh: vehicles) {
            if( !veh->getRoute().empty() && !veh->getAtDest() ) {

                Vector<Road*> route = veh->getRoute();
                if (route[veh->getIndex()]->getDest()->getName() == veh->getEnd()->getName()) {
                    cout<<"Reached Destination"<<endl;
                    veh->setAtDest(true);
                }
                else {
                    //Making key (can also add a var in vehicle class for current key)
                    string key="";
                    if(veh->getIndex()==0) {
                        key+=veh->getStart()->getName();
                        // cout<<"no "<<veh->getIndex()<<endl;
                        key+=route[veh->getIndex()]->getDest()->getName();
                    }
                    else {
                        key+=route[veh->getIndex()-1]->getDest()->getName();
                        key+=route[veh->getIndex()]->getDest()->getName();
                    }

                    //Updating road
                    veh->moveIndex();

                    cout<<veh->getName()<<endl;
                    cout<<"Key "<<key[0]<<" to "<<key[1]<<endl;

                    cout<<"current Road "<<route[veh->getIndex()-1]->getDest()->getName()<<" to "<<route[veh->getIndex()]->getDest()->getName();
                    cout<<endl;

                    // Updating HashMap
                    roadVehicleMap.search(key)->removeByValue(veh);
                    key="";
                    key+=route[veh->getIndex()-1]->getDest()->getName();
                    key+=route[veh->getIndex()]->getDest()->getName();

                    if (roadVehicleMap.find(key)) {
                        roadVehicleMap.search(key)->insertAtStart(veh);
                    } 
                    else {
                        LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                        newList->insertAtStart(veh);
                        roadVehicleMap.insert(key, newList);
                    }
                }

            }
            else{
                // cout<<"No path Found\n";
            }
        
        }
        
        for (Intersection* intsc: intersections) {
            intsc->updateSignals();
        }
        //displaying HashMap
        display_Vehicles_at_Roads();
    }

    //Displaying Road Status
    void display_Roads_Status() {
        for (Intersection* intsc : intersections) {
            LinkedList<Road*>& list = graph[intsc->getName()-'A'];
            for (LinkedList<Road*>::Node* node = list.getHead(); node != nullptr; node = node->next) {
                Road* rd = node->data;
                cout<<intsc->getName()<<" to "<<rd->getDest()->getName()<<" status: "<<rd->getStatus()<<endl;
            }
        }
    }
    
    //Displaying Vehicles on each road
    void display_Vehicles_at_Roads(){
        cout<<"Congestion Status"<<endl;
        for(Intersection* intsc : intersections){
            char name=intsc->getName();
            LinkedList<Road*>::Node* node=graph[name-'A'].getHead();
            while(node) {
                string key="";
                key+=name;
                key+=node->data->getDest()->getName();
                if(roadVehicleMap.find(key)) {
                    LinkedList<Vehicle*>* temp_veh = roadVehicleMap.search(key);
                    LinkedList<Vehicle*>::Node* veh_head = temp_veh->getHead();
                    
                    // getting which vehicles have reached dest.
                    Vector<Vehicle*> tempVs;
                    while (veh_head) {
                        if (veh_head->data->getAtDest())
                            tempVs.push_back(veh_head->data);
                        veh_head = veh_head->next;
                    }

                    // removing vehicles that have reached dest.
                    for (Vehicle* veh: tempVs) {
                        temp_veh->removeByValue(veh);
                    }

                    veh_head= temp_veh->getHead();

                    cout<<"Vehicles in Path "<<key[0]<<" to "<<key[1]<<" are "<<temp_veh->getSize()<<" : ";
                    
                    while (veh_head) {
                        cout<<veh_head->data->getName()<<" ";
                        veh_head = veh_head->next;
                    }
                    cout<<endl;

                }
                node=node->next;

            }
        }
    }

    void displayGraph(RenderWindow& window, int x,int y) {
        int i=0;
        cout<<"City Graph System"<<endl;
        for (Intersection* intsc : intersections) {
            char name = intsc->getName();
            cout<<name<<": ";
            graph[name-'A'].printList();
        }
        cout<<endl;
        //     // if(!graph[name- 'A'].isEmpty()){
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
            
        // }
        display_Roads_Status();
        display_Vehicles_at_Roads();
    }
};


// clear route after getting to dest
// display all possible paths between intersections with weights.
// signals
// emergency vehicle integration

// TO DO:
// add all paths in hashMap whether they're blocked or not.
// show blocked paths, clear paths