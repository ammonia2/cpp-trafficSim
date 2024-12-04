#include "vehicle.h"
#include "vector.h"

void Vehicle::moveIndex() {
    if (name == "V17")
        cout<<"\n\nname\n"<<endl;

    currRoadIndex+=1;
    setRoad();
}

void Vehicle::setRoad() {
    if( route.size()!=0 && (currRoadIndex < route.size() && !atDest) ) {
        cout<<"entered "<<route.size()<<" Curr:"<<currRoadIndex<<endl;
        travelTime = route[currRoadIndex]->getWeight();
        route[currRoadIndex]->insertVehicle(this);
    }
}