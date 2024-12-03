#include "vehicle.h"
#include "vector.h"

void Vehicle::moveIndex() {
    currRoadIndex+=1;
    setRoad();
}

void Vehicle::setRoad() {
    travelTime = route[currRoadIndex]->getWeight();
    route[currRoadIndex]->insertVehicle(this);
}