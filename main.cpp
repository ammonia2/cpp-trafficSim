// g++ main.cpp classes/intersection.cpp classes/vehicle.cpp -o app -lsfml-graphics -lsfml-window -lsfml-system && ./app

#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

#include "classes/hash_table.h"
#include "classes/list.h"
#include "classes/priority_queue.h"
#include "classes/queue.h"
#include "classes/stack.h"
#include "classes/adjacency_list.h"
#include "classes/DynamicArray.h"

using namespace std;
using namespace sf;

// CLASSES STRUCTURE:
//  class Traffic Signal: (not all nodes have traffic signals)
//      green time, yellow 3, red: total (can be assumed 60) - green - yellow

//  class Intersection (Node):
//      DynamicArray incoming roads
//      DynamicArray outgoing roads
//      traffic Signal

//  class Road:
//      length, congestion level
//      DynamicArray vehicles

//  class Vehicle:
//      road, position on road
//      progress of route
//      start, End Intersection

//  class Emergency Vehicle (inherits from Vehicle):
//      different priority, dynamic routing (next road will be calculated at every intersection)

//  class TrafficManagement:
//      list of intersections,
//      list of roads, vehicles,
//      roads and vehicles stored in an adjacency list
//      clock for updating vehicles

// #include <iostream>

// int main() {
//     // ANSI escape codes for text colors
//     std::cout << "\033[1;31mThis is red text.\033[0m\n";  // Red
//     std::cout << "\033[1;32mThis is green text.\033[0m\n"; // Green
//     std::cout << "\033[1;33mThis is yellow text.\033[0m\n"; // Yellow
//     std::cout << "\033[1;34mThis is blue text.\033[0m\n"; // Blue
//     std::cout << "\033[1;35mThis is magenta text.\033[0m\n"; // Magenta
//     std::cout << "\033[1;36mThis is cyan text.\033[0m\n"; // Cyan
//     std::cout << "\033[1;37mThis is white text.\033[0m\n"; // White

//     return 0;
// }

class Road;
class Vehicle;
class AdjacencyList;
class TrafficSignal;

class TrafficManagement{

    DynamicArray<Intersection*> intersection;
    DynamicArray<Road*> roads;
    DynamicArray<Vehicle*> vehicles;
    AdjacencyList graph;
    Clock clock;

    public:

    void startSimulation() {
        static auto lastUpdate = chrono::steady_clock::now();

        while (true) {
            auto now = chrono::steady_clock::now();

            if (chrono::duration_cast<chrono::seconds>(now - lastUpdate).count() >= 1) { // 1sec
                graph.updateSimulation();
                lastUpdate = now;
            }

            this_thread::sleep_for(chrono::milliseconds(10)); // to check in periods of 0.2?
            // will maybe cause probs if mutliple calculations needed per second
        }
    }

    Intersection* getIntersection(char A) {
        return graph.getIntersection(A);
    }

    void displayCongestionStatus() {
        graph.display_Vehicles_at_Roads();
    }
    
    void displaySignalStatus() {
        graph.displaySignals();
    }

    void displayCityTrafficNetwork() { // adjacency list
        graph.displayGraph();
    }

    void displayBlockedRoads() {

        // for (int i = 0; i < roads.size(); i++) {

        //     if (roads[i]->getStatus() == "Blocked") {

        //         // cout << "Blocked Road: " << roads[i]->getStartInt()->getName()
        //         // << " -> " << roads[i]->get()->getName() << endl;
        //     }
        // }
    }

    void blockRoad() {
        char start, end;
        cout<<"Enter Start Intersection of the road to Block: ";
        cin>>start;
        cout<<"Enter End Intersection of the road to Block: ";
        cin>>end;

        Road* road = graph.findRoad(start, end);
        if (road) {
            road->setStatus("Blocked");
            graph.clearQueues();
            graph.initialiseRoutes(start, road);
            return;
        }
        cout<<"Road Not Found!\n";
    }

    void addVehicle(Vehicle* veh, bool ev=0) {
        if (!ev)
            graph.appendVehicle(veh);
        else
            graph.appendEV(veh);
    }

};

int main() {
    TrafficManagement obj;
    
    cout<<"-----------------Simulation Dashboard------------------------\n";
    cout<<"1. Display City Traffic Network\n";
    cout<<"2. Display Traffic Signal Status\n";
    cout<<"3. Display Congestions Status\n";
    cout<<"4. Display Bloacked Roads\n";
    cout<<"5. Block a Road\n";
    cout<<"6. Add a Vehicle/Emergency Vehicle\n";
    cout<<"7. Simulate Vehicle Routing\n";
    cout<<"8. Exit Simulation\n";

    int choice;
    cin>>choice;
    string name;

    Intersection* s, *e;

    while (choice <=8 && choice >=1) {
        switch (choice) {
            case 1:
                obj.displayCityTrafficNetwork();
                break;
            case 2:
                obj.displaySignalStatus();
                break;
            case 3:
                obj.displayCongestionStatus();
                break;
            case 4:
                obj.displayBlockedRoads();
                break;
            case 5:
                obj.blockRoad();
                break;
            case 6:
                cout<<"Emergency Vehicle? (Y/N): ";
                char choice;
                cin>>choice;

                cout<<"Enter Vehicle Name: ";
                cin>>name;

                char start, end;
                cout<<"Enter Start and End Road: ";
                cout<<"Enter Start and End Road: ";
                cin>>start;
                cin>>end;

                s = obj.getIntersection(start);
                e = obj.getIntersection(end);

                if (choice=='y' || choice == 'Y') {
                    string priority;
                    cout<<"Enter Vehicle priority: ";
                    cin>>priority;

                    EmergencyVehicle* ev = new EmergencyVehicle(name, s, e, priority);
                    obj.addVehicle(ev, 1);
                }
                else {
                    Vehicle* v = new Vehicle(name, s, e, 0);
                    obj.addVehicle(v);
                }
                break;
            case 7:
                obj.startSimulation();
                break;
            case 8:
                cout<<"Try again\n";
            default:
                cout<<"\n-------------- Exiting --------------\n";
                return 0;
                break;
        }
        
        cout<<"-----------------Simulation Dashboard------------------------\n";
        cout<<"1. Display City Traffic Network\n";
        cout<<"2. Display Traffic Signal Status\n";
        cout<<"3. Display Congestions Status\n";
        cout<<"4. Display Bloacked Roads\n";
        cout<<"5. Block a Road\n";
        cout<<"6. Add a Vehicle/Emergency Vehicle\n";
        cout<<"7. Simulate Vehicle Routing\n";
        cout<<"8. Exit Simulation\n";
        cin>>choice;
    }
}