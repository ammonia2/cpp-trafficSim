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

    void initialiseEmergencyVehicles() {
        for (EmergencyVehicle* vehicle : emergencyVehicles) {
            vehicles.push_back(vehicle);
            
            // Calculate initial path using A* algorithm
            Vector<char> refs = aStarAlgo(vehicle->getStart(), vehicle->getEnd());
            Vector<char> path = constructPath(refs, vehicle->getStart(), vehicle->getEnd());
            
            if (!path.empty() && path.size() > 1) {
                // Only process the first road in the path
                char currentNode = path[0];
                char nextNode = path[1];
                
                // Find the corresponding road
                Road* road = nullptr;
                LinkedList<Road*>& edges = graph[currentNode - 'A'];
                LinkedList<Road*>::Node* edge = edges.getHead();
                
                while (edge) {
                    if (edge->data->getDest()->getName() == nextNode) {
                        road = edge->data;
                        break;
                    }
                    edge = edge->next;
                }
                
                if (road) {
                    vehicle->addRoad(road);
                    vehicle->setRoad();
                    vehicle->setOld(vehicle->getStart()->getName());
                    
                    // Add to roadVehicleMap
                    string key = "";
                    key += currentNode;
                    key += nextNode;
                    
                    if (roadVehicleMap.find(key)) {
                        roadVehicleMap.search(key)->insertAtStart(vehicle);
                    } else {
                        LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                        newList->insertAtStart(vehicle);
                        roadVehicleMap.insert(key, newList);
                    }
                }
            }
        }
    }

    void initialiseVehicles() {

        for (Vehicle* vehicle: vehicles) {
            Vector<char> refs = dijkstraAlgo(vehicle->getStart(), vehicle->getEnd());
            Vector<char> path = constructPath(refs, vehicle->getStart(), vehicle->getEnd());
            
            if(!path.empty()) {
                // cout<<path.front()<<" to "<<path.back()<<" : ";
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

                vehicle->setRoad();
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

            if (!weight.empty() && weight.back() == '\r') {
                weight.pop_back();
            }
        
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
        initialiseEmergencyVehicles();

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
        }
        return path;
    }

    // exact way to measure heuristic is to calculate the travelTime from the edge to the ending edge. 
    // (could also require checking congestion (secondary weight))

    Vector<char> aStarAlgo(Intersection* start, Intersection* end) {

        // Initialize vectors and priority queue
        Vector<int> distance; // g(n): cost to reach the node
        Vector<int> heuristic; // h(n): estimated cost to goal
        Vector<char> previousReferences; // Tracks the path
        Vector<bool> visited;
        PriorityQueue<DistanceNode> pq(true, intersections.size()); // Min-heap

        distance.resize(intersections.size());
        heuristic.resize(intersections.size());
        previousReferences.resize(intersections.size());
        visited.resize(intersections.size());

        for (int i = 0; i < visited.size(); i++) {
            visited[i] = false;
            distance[i] = INT_MAX;
            previousReferences[i] = '\0';
            heuristic[i] = 0; // Default heuristic
        }

        // Heuristic: travelTime of roads connected to the goal
        for (Intersection* intsc : intersections) {
            
            char name = intsc->getName();
            LinkedList<Road*>& edges = graph[name - 'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();

            int minTravelTime = INT_MAX;
            
            while (edge) {
                Road* road = edge->data;
                if (road->getStatus() == "Clear") {
                    int travelTime = road->getWeight() + road->getTrafficLoad(); // Adjusted for congestion
                    if (travelTime < minTravelTime) {
                        minTravelTime = travelTime;
                    }
                }
                edge = edge->next;
            }
            heuristic[name - 'A'] = (minTravelTime == INT_MAX) ? 0 : minTravelTime;
        }

        // Initialize the starting node
        distance[start->getName() - 'A'] = 0;
        pq.push(DistanceNode(start, heuristic[start->getName() - 'A'])); // f(n) = g(n) + h(n)

        // Main A* loop
        while (!pq.isEmpty()) {
            DistanceNode minNode = pq.top();
            pq.pop();

            Intersection* minIntersection = minNode.intersection;
            char minNodeName = minIntersection->getName();

            if (visited[minNodeName - 'A'])
                continue;

            visited[minNodeName - 'A'] = true;

            // Stop early if the goal is reached
            if (minIntersection == end)
                break;

            LinkedList<Road*>& edges = graph[minNodeName - 'A'];
            LinkedList<Road*>::Node* edge = edges.getHead();

            while (edge) {
                Road* road = edge->data;
                if (road->getStatus() == "Clear") {
                    Intersection* neighbour = road->getDest();
                    
                    // Adjust travel time for congestion
                    int travelTime = road->getWeight() + road->getTrafficLoad();

                    int tempDistance = distance[minNodeName - 'A'] + travelTime; 

                    if (tempDistance < distance[neighbour->getName() - 'A']) {
                        distance[neighbour->getName() - 'A'] = tempDistance;
                        previousReferences[neighbour->getName() - 'A'] = minNodeName;

                        // Calculate f(n) = g(n) + h(n)
                        int priority = tempDistance + heuristic[neighbour->getName() - 'A'];
                        pq.push(DistanceNode(neighbour, priority));
                    }
                }
                edge = edge->next;
            }
        }

        return previousReferences;
    }

    // set a 1 sec delay in addition of vehicles in vehicle array so each car appears
    // behind the other and no collision happens to occur
    void updateSimulation() {

        Vector<Vehicle*> atRoadEnd;
        Vector<Road*> doneRoads;
        

        for(Vehicle* veh: vehicles) {
            
            if( !veh->getRoute().empty() && !veh->getAtDest() ) {
                Vector<Road*> route = veh->getRoute();
                cout<<veh->getName()<<endl;
                cout<<"route size: "<<route.size()<<" current index: "<<veh->getIndex()<<" Time: "<<veh->getTime()<<endl;


                if (route.size()==veh->getIndex() && veh->getTime() <= 1 ) {
                    Intersection* currentIntersection =  route[veh->getIndex() - 1]->getDest();
                    if (veh->getName()[0] == 'E') {
                        cout<<endl<<endl<<currentIntersection->getName()<<" "<<veh->getEnd()->getName()<<endl<<endl;
                    }

                    if(currentIntersection->getName() == veh->getEnd()->getName()) {
                        cout << "Reached Destination" << endl;
                        veh->setAtDest(true);
                    }
                    else {
                        // Need to find next path
                        cout<<"\n\nev bad\n";
                        string oldKey="";
                        oldKey+=veh->getOld();
                        oldKey+=currentIntersection->getName();

                        Vector<char> refs = aStarAlgo(currentIntersection, veh->getEnd());
                        Vector<char> newPath = constructPath(refs, currentIntersection, veh->getEnd());
                        cout<<"New Path "; newPath.display();
                        
                        if(!newPath.empty() && newPath.size() > 1) {
                            char currentNode = newPath[0];
                            char nextNode = newPath[1];
                            Road* nextRoad = nullptr;

                            roadVehicleMap.search(oldKey)->removeByValue(veh);
                            
                            LinkedList<Road*>& edges = graph[currentNode - 'A'];
                            LinkedList<Road*>::Node* edge = edges.getHead();
                            
                            while(edge) {
                                if(edge->data->getDest()->getName() == nextNode && 
                                edge->data->getStatus() == "Clear") {
                                    nextRoad = edge->data;
                                    break;
                                }
                                edge = edge->next;
                            }

                            if(nextRoad) {
                                veh->clearRoute();
                                veh->addRoad(nextRoad);
                                veh->setTime(nextRoad->getWeight());
                                veh->setIndex(1);

                                veh->setOld(currentIntersection->getName());
                                route = veh->getRoute();
                                route.display();

                                // Update hashmap
                                string newKey = "";
                                newKey += currentNode;
                                newKey += nextNode;

                                if(roadVehicleMap.find(newKey)) {
                                    roadVehicleMap.search(newKey)->insertAtStart(veh);
                                } else {
                                    LinkedList<Vehicle*>* newList = new LinkedList<Vehicle*>();
                                    newList->insertAtStart(veh);
                                    roadVehicleMap.insert(newKey, newList);
                                }
                            }
                        }
                    }
                }

                else {
                    if(veh->getName()=="EV3"){
                        cout<<veh->getTime()<<" "<<veh->getIndex()<<endl;
                    }
                    //Making key (can also add a var in vehicle class for current key)
                    string key="69";

                    //Updating road
                    veh->updateTime();
                    // cout<<veh->getIndex()<<" "<<veh->getRoute().size()<<endl;
                    Road* road = veh->getRoute()[veh->getIndex()];
            
                    if (veh->getTime() == 0) {
                        // doneRoads.clear();
                        atRoadEnd.push_back(veh);
                    }
                    
                    int temp_idx=veh->getIndex();
                    if (!doneRoads.contains(road)) {
                        if ( veh->getTime()==0 && road->getDest()->signalActive(road)) {

                            veh = road->getHeapTop();
                            veh = road->getHeapTop();
                            // if (veh == nullptr) {
                            //     cout << "Error: No vehicle found at the top of the heap!\n\n\n\n\n\n\n\n\n\n";
                            //     continue;  // Skip this iteration
                            // }

                            route = veh->getRoute();
                            temp_idx=veh->getIndex();
                            key="";
                            if(veh->getIndex()==0) {
                                key+=veh->getStart()->getName();
                                key+=route[veh->getIndex()]->getDest()->getName();
                            }
                            else {
                                key+=route[veh->getIndex() - 1]->getDest()->getName();
                                key+=route[veh->getIndex()]->getDest()->getName();
                            }
                            
                            doneRoads.push_back(road);
                            veh->moveIndex();
                            road->removeVehicle();
                        }
                    }

                    cout<<"Key "<<key[0]<<" to "<<key[1]<<" TIME : "<<veh->getTime()<<endl;

                    // Updating HashMap
                    cout<<veh->getName()<<endl;
                    cout<<"prev: "<<temp_idx<<"  Updated: "<<veh->getIndex()<<endl;

                    // FINDING NEXT ROAD
                    // need to add the functionality that if the next road is blocked, then the 
                    // route needs to be recalculated from that intersection (don't forget to
                    // delete the roads ahead of that!)
                    if( veh->getIndex()>temp_idx && route.size()!=veh->getIndex() ) {
                        cout<<"current Road "<<route[veh->getIndex()-1]->getDest()->getName()<<" to "<<route[veh->getIndex()]->getDest()->getName();
                        
                        cout<<" veh: "<<veh->getName()<<" key: "<<key<<endl;
                        roadVehicleMap.search(key)->removeByValue(veh);
                        cout<<veh->getName()<<"  Previous key: "<<key<<"  ";

                        key="";
                        key+=route[veh->getIndex() - 1]->getDest()->getName();
                        key+=route[veh->getIndex()]->getDest()->getName();
                        cout<<"New key: "<<key<<endl;
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

            }
            else {
                // cout<<"No path Found\n";
            }
        
        }

        for (Intersection* intsc: intersections) {
            intsc->updateSignals();
        }

        // displaying HashMap
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
                    cout<<"Signal: "<<node->data->getDest()->getSignal(node->data)<<" Time: "<<node->data->getDest()->getSignalTime(node->data)<<" Vehicles in Path "<<key[0]<<" to "<<key[1]<<" are "<<temp_veh->getSize()<<" : ";
                    
                    while (veh_head) {
                        cout<<veh_head->data->getName()<<" ( " << veh_head->data->getPriorityLevel() << ", "<< veh_head->data->getTime() << " ) ";
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


// display all possible paths between intersections with weights.
// signals
// emergency vehicle integration

// TO DO:
// add all paths in hashMap whether they're blocked or not.
// show blocked paths, clear paths