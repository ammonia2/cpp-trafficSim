#include<iostream>
using namespace std;

// template<typename T>
class traffic_Signal_Node{
public:
    int total;
    int green, yellow, red;
    traffic_Signal_Node(int green=0):total(60),green(green),yellow(3){
        red=total-green+yellow;
    }
 };
