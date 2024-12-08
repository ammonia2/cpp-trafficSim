// command to run:
// g++ -o app main.cpp classes/intersection.cpp classes/vehicle.cpp && ./app


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
using namespace std::chrono;

class Road;
class Vehicle;
class AdjacencyList;
class TrafficSignal;

class TrafficManagement {
    DynamicArray<Intersection*> intersection;
    DynamicArray<Road*> roads;
    DynamicArray<Vehicle*> vehicles;
    AdjacencyList graph;

public:
    void startSimulation(int duration = -1) {
        int time = 0;
        auto lastUpdate = steady_clock::now();
        graph.setSimultaionBool(true);
        if (!graph.getInitialised()) {
            graph.initialiseRoutes();
            graph.setInitialised(true);
        }
        while (true) {
            auto now = steady_clock::now();
            if (duration_cast<seconds>(now - lastUpdate).count() >= 1) { // 1 sec
                graph.updateSimulation();
                lastUpdate = now;
                time += 1;
            }

            this_thread::sleep_for(milliseconds(10));
            
            // will maybe cause probs if multiple calculations needed per second
            if (duration != -1 && time >= duration || graph.getSimulationBool() == false) {
                return;
            }
            else {
                if (!graph.hasVehiclesOnRoads()) return;
            }
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
        graph.displayBlockedRoads();
    }

    void blockRoad() {
        char start, end;
        cout << "Enter Start Intersection of the road to Block: ";
        cin >> start;
        cout << "Enter End Intersection of the road to Block: ";
        cin >> end;

        Road* road = graph.findRoad(start, end);
        if (road && road->getTrafficLoad() >0) {
            cout<<"Vehicles present in the road. Can't block it yetn\n";
        }
         if (road) {
            road->setStatus("Blocked");
            return;
        }
        cout << "Road Not Found!\n";
    }

    void addVehicle(Vehicle* veh, bool ev = 0) {
        if (!ev)
            graph.appendVehicle(veh);
        else
            graph.appendEV(veh);
    }
};

int main() {
    TrafficManagement obj;

    cout << "-----------------Simulation Dashboard------------------------\n";
    cout << "1. Display City Traffic Network\n";
    cout << "2. Display Traffic Signal Status\n";
    cout << "3. Display Congestions Status\n";
    cout << "4. Display Blocked Roads\n";
    cout << "5. Block a Road\n";
    cout << "6. Add a Vehicle/Emergency Vehicle\n";
    cout << "7. Simulate Vehicle Routing\n";
    cout << "8. Exit Simulation\n";

    int choice;
    cin >> choice;
    string name;

    Intersection* s, *e;

    while (choice <= 8 && choice >= 1) {
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
                cout << "Emergency Vehicle? (Y/N): ";
                char choice;
                cin >> choice;

                cout << "Enter Vehicle Name: ";
                cin >> name;

                char start, end;
                cout << "Enter Start and End Road: ";
                cin >> start;
                cin >> end;

                s = obj.getIntersection(start);
                e = obj.getIntersection(end);

                if (choice == 'y' || choice == 'Y') {
                    string priority;
                    cout << "Enter Vehicle priority: ";
                    cin >> priority;

                    EmergencyVehicle* ev = new EmergencyVehicle(name, s, e, priority);
                    obj.addVehicle(ev, 1);
                } else {
                    Vehicle* v = new Vehicle(name, s, e, 0);
                    obj.addVehicle(v);
                }
                break;
            case 7:
                cout << "Simulate for a Specific Time? (Y/N): ";
                char ch;
                cin >> ch;
                if (ch == 'y' || ch == 'Y') {
                    int t = 0;
                    cout << "Enter Time(sec): ";
                    cin >> t;
                    obj.startSimulation(t);
                } else {
                    obj.startSimulation();
                }
                break;
            case 8:
                cout << "Try again\n";
            default:
                cout << "\n-------------- Exiting --------------\n";
                return 0;
                break;
        }

        cout << "-----------------Simulation Dashboard------------------------\n";
        cout << "1. Display City Traffic Network\n";
        cout << "2. Display Traffic Signal Status\n";
        cout << "3. Display Congestions Status\n";
        cout << "4. Display Blocked Roads\n";
        cout << "5. Block a Road\n";
        cout << "6. Add a Vehicle/Emergency Vehicle\n";
        cout << "7. Simulate Vehicle Routing\n";
        cout << "8. Exit Simulation\n";
        cin >> choice;
    }
}