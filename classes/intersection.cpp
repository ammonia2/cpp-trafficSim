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
    for (TrafficSignal& signal : signals) {
        signal.update();
    }
}
