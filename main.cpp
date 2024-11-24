// g++ main.cpp -o app -lsfml-graphics -lsfml-window -lsfml-system && ./app
#include <SFML/Graphics.hpp>
#include <iostream>
#include "classes/hash_table.h"
#include "classes/list.h"
#include "classes/priority_queue.h"
#include "classes/queue.h"
#include "classes/stack.h"
using namespace std;

// CLASSES STRUCTURE:
//  class Traffic Signal: (not all nodes have traffic signals)
//      green time, yellow 3, red: total (can be assumed 60) - green - yellow

//  class Intersection (Node):
//      vector incoming roads
//      vector outgoing roads
//      traffic Signal

//  class Road:
//      length, congestion level
//      vector vehicles

//  class Vehicle:
//      road, position on road
//      progress of route
//      start, End Intersection

//  class Emergency Vehicle (inherits from Vehicle):
//      different priority, dynamic routing (next road will be calculated at every intersection)

//  class TrafficManagement:
//      list of intersections,
//      list of roads, vehicles,
//      roads and vehicles stored in an adjacency list
//      clock for updating vehicles