#include "intersection.h"
#include "road.h"

void Intersection::addOutRoad(Road* r) {
    outRoads.push_back(r);
}

void Intersection::addInRoad(Road* r) {
    inRoads.push_back(r);
}

void Intersection::calculateSignalTimings() {
    if (inRoads.size() == 0) {
        cout << "Intersection " << name << " has no incoming roads!" << endl;
        return;
    }

    int totalCycleTime = 0;

    for (Road* road : inRoads) {
        int roadGreenTime = greenTime;
        totalCycleTime += (roadGreenTime + 3); // 3 seconds for yellow time
    }

    signals.clear();

    for (Road* road : inRoads) {
        int roadGreenTime = greenTime;
        int roadRedTime = totalCycleTime - (roadGreenTime + 3);

        TrafficSignal signal(roadGreenTime, roadRedTime);
        signals.push_back(signal);

        cout << "Signal for road to " << road->getDest()->getName()
             << " set to " << roadGreenTime << " seconds. RED: " << roadRedTime << endl;
    }
}

void Intersection::updateSignals() {
    static int currentSignalIndex = 0; // Tracks the current green signal

    // Update the current green signal
    if (!signals.empty()) {
        TrafficSignal& currentSignal = signals[currentSignalIndex];
        currentSignal.update();

        // If the current green signal completes its red cycle, move to the next signal
        if (currentSignal.getState() == "RED") {
            currentSignalIndex = (currentSignalIndex + 1) % signals.size();
            TrafficSignal& nextSignal = signals[currentSignalIndex];

            // Set the next signal to green
            nextSignal.setState("GREEN");
            nextSignal.resetTime();
        }

        // Ensure all other signals are red
        cout<<"\nIntersection: "<<name<<endl;
        for (int i = 0; i < signals.size(); i++) {
            if (i != currentSignalIndex) {
                TrafficSignal& otherSignal = signals[i];
                if (otherSignal.getState() != "RED") {
                    otherSignal.setState("RED");
                    otherSignal.resetTime();
                }
            }
            cout<<"Signal "<<i<<signals[i].getState()<<endl;
        }
    }
}
