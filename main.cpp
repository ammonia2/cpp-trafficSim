// g++ main.cpp -o app -lsfml-graphics -lsfml-window -lsfml-system && ./app
#include <SFML/Graphics.hpp>
#include <iostream>

#include "classes/hash_table.h"
#include "classes/list.h"
#include "classes/priority_queue.h"
#include "classes/queue.h"
#include "classes/stack.h"
#include "classes/adjacency_list.h"

using namespace std;
using namespace sf;

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

//SFML code for help 

// #include <SFML/Graphics.hpp>

// // Bring everything from the sf namespace into the global scope
// using namespace sf;

// int main() {
//     // Now we can use classes without the 'sf::' prefix
//     RenderWindow window(VideoMode(800, 600), "SFML Basic Example");
//     RectangleShape rectangle(Vector2f(200.f, 100.f));
//     rectangle.setFillColor(Color::Green);
//     rectangle.setPosition(100.f, 100.f);

//     CircleShape circle(50.f);
//     circle.setFillColor(Color::Blue);
//     circle.setPosition(400.f, 200.f);

//     Font font;
//     if (!font.loadFromFile("path/to/font.ttf")) {
//         return -1;
//     }

//     Text text("Hello, SFML!", font, 30);
//     text.setFillColor(Color::Red);
//     text.setPosition(300.f, 400.f);

//     while (window.isOpen()) {
//         Event event;
//         while (window.pollEvent(event)) {
//             if (event.type == Event::Closed) {
//                 window.close();
//             }
//         }

//         window.clear(Color::Black);
//         window.draw(rectangle);
//         window.draw(circle);
//         window.draw(text);
//         window.display();
//     }

//     return 0;
// }


class Road;
class Vehicle;
class AdjacencyList;
class TrafficSignal;

class TrafficManagement{ // to be completed by Hasaan the artist 😘💕🙌

    vector<Intersection*> intersection;
    vector<Road*> roads;
    vector<Vehicle*> vehicles;
    AdjacencyList graph;
    Clock clock;

    public: // store pointers in vectors plems
    void management(RenderWindow& window){
        graph.displayGraph(window,400,300);
    }


};

int main() {
    RenderWindow window(VideoMode(800, 600), "SFML Basic Example");
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
        }
        TrafficManagement obj;
        obj.management(window);
        // window.clear();
        window.display();
    }
    // AdjacencyList graph;
    // graph.displayGraph();
}