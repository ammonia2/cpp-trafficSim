#include<iostream>
#include<vector>
#include "traffic_signal.h"
using namespace std;

template<typename T>
 class Intersection_Node{
public:
    vector<T>incoming_roads;
    vector<T>outgoing_roads;
    traffic_Signal_Node signal;
    Intersection_Node(){}
 };