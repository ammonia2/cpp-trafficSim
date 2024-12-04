#include "vehicle.h"
#include "vector.h"

void Vehicle::moveIndex() {
    if(getName()=="V7"){
    }
    // if(getTime()==0){
        cout<<"adding currRoad\n";
        currRoadIndex+=1;
        cout<<"added currRoad\n";
        setRoad();
        cout<<"calling sets\n";
    // }
}

void Vehicle::setRoad() {
    cout<<"pushing at \n";
    if( route.size()!=0 && (currRoadIndex < route.size() && !atDest) ) {
        cout<<"entered "<<route.size()<<" Curr:"<<currRoadIndex<<endl;
        travelTime = route[currRoadIndex]->getWeight();
        cout<<"pushing at 2\n";
        route[currRoadIndex]->insertVehicle(this);
        cout<<"pushing at 3\n";
    }
}