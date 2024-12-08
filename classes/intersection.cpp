#include "intersection.h"
#include "road.h"

void Intersection::addOutRoad(Road* r) {
    outRoads.push_back(r);
}

void Intersection::addInRoad(Road* r) {
    inRoads.push_back(r);
}

string Intersection:: getSignal(Road* road){
    for (int i=0; i<inRoads.size(); i++ ) {
        
        if (inRoads[i] == road) {
            return signals[i].getState();
        }
    }
    return "false";
}

void Intersection::calculateSignalTimings() {
    if (inRoads.size() == 0) {
        return;
    }

    int totalCycleTime = inRoads.size() * (greenTime + 3); // Total cycle time including yellow time

    signals.clear();

    if (inRoads.size() == 1) { // signal will always be green if only 1 incoming road
        Road* road = inRoads[0];
        TrafficSignal signal(greenTime, 0);
        signal.setYellowTime(0);
        signal.setState("GREEN");
        signal.resetTime();
        signals.push_back(signal);
    }
    else {
        for (int i = 0; i < inRoads.size(); ++i) {
            Road* road = inRoads[i];
            int roadRedTime = totalCycleTime - (greenTime );

            TrafficSignal signal(greenTime, roadRedTime);
            if (i == 0) {
                signal.setState("GREEN");
                signal.resetTime();
            } else {
                signal.setState("RED");
                signal.setCurrentTime(totalCycleTime - (i * (greenTime + 3)));
            }
            signals.push_back(signal);

            // cout << "Signal for road to " << road->getDest()->getName()
            //     << " set to GREEN: " << greenTime << " seconds, RED: " << roadRedTime << " seconds. Current Red: " 
            //     << (totalCycleTime - (i* (greenTime+3))) <<  endl;
        }
    }
}

void Intersection::updateSignals() {
    // Updating all signals
    for (TrafficSignal& signal : signals) {
        signal.update();
    }

    if (signals.size() <= 1) return;

    // Identifying the current green signal
    int currentGreenIndex = -1;
    for (int i = 0; i < signals.size(); ++i) {
        if (signals[i].getState() == "GREEN") {
            currentGreenIndex = i;
            break;
        }
    }

    if (currentGreenIndex == -1) {
        return;
    }

    Road* currentGreenRoad = inRoads[currentGreenIndex];

    if (currentGreenRoad->getTrafficLoad() == 0) {

        int maxLoad = -1;
        int highestLoadIndex = -1;

        for (int i = 0; i < inRoads.size(); ++i) {
            int currentLoad = inRoads[i]->getTrafficLoad();
            if (currentLoad == 0) continue;

            // only if there's a vehicle ready to move
            Vehicle* vehicle = inRoads[i]->getHeapTop();
            if (currentLoad > maxLoad && vehicle && vehicle->getTime() == 0) {
                maxLoad = currentLoad;
                highestLoadIndex = i;
            }
        }


        // Switch signals only if a more congested road
        if (highestLoadIndex != -1 && highestLoadIndex != currentGreenIndex) {
            int totalCycleTime = inRoads.size() * (greenTime + 3);

            for (int i = 0; i < inRoads.size(); ++i) {
                int idx = (highestLoadIndex + i) % inRoads.size();

                if (i == 0) {
                    signals[idx].setState("GREEN");
                    signals[idx].resetTime();
                } else {
                    signals[idx].setState("RED");
                    signals[idx].setCurrentTime(totalCycleTime - (i * (greenTime + 3)));
                }
            }

            cout<<"\n\033[1;33Changed signal timings for intersection: "<<name<<"\033[0m\n";
        }
    }
}

