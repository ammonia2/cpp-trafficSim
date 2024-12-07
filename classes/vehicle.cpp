#include "vehicle.h"
#include "DynamicArray.h"

void Vehicle::moveIndex() {
    currRoadIndex+=1;
    setRoad();
}

void Vehicle::setRoad() {
    if( route.size()!=0 && (currRoadIndex < route.size() && !atDest) ) {
        travelTime = route[currRoadIndex]->getWeight();
        route[currRoadIndex]->insertVehicle(this);
    }
}