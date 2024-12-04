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

    int totalCycleTime = inRoads.size() * (greenTime + 3); // Total cycle time including yellow time

    signals.clear();

    if (inRoads.size() == 1) { // signal will always be green if only 1 incoming road
        Road* road = inRoads[0];
        TrafficSignal signal(greenTime, 0);
        signal.setYellowTime(0);
        signal.setState("GREEN");
        signal.resetTime();
        signals.push_back(signal);
        cout << "Signal for road to " << road->getDest()->getName()
             << " set to GREEN: " << signal.getGreenTime() << " seconds, RED: 0 seconds." << endl;
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
        cout <<name<<"  T: "<< signal.getTimeRemaining()<<" Green Time: "<<signal.getGreenTime()<<". ";
        
    }

    // for (int i = 0; i < signals.size(); ++i) {
    //     cout <<name<<"  T: "<< signals[i].getTimeRemaining()<<" Green Time: "<<signals[i].getGreenTime()<<". ";
    // }
}
