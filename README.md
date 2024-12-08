# Traffic Management Simulation


## Overview

The *Traffic Management Simulation* is a comprehensive C++ application designed to model and simulate traffic flow within a city network. It incorporates various data structures and algorithms to manage intersections, roads, vehicles (including emergency vehicles), and traffic signals. The simulation provides real-time updates on traffic conditions, signal statuses, vehicle movements, and handles dynamic scenarios such as road blockages and emergency routing.

## Features

- *Graph-Based City Network:* Utilizes an adjacency list to represent intersections and roads.
- *Dynamic Traffic Signals:* Traffic signals at intersections change states (GREEN, YELLOW, RED) based on configurable timings.
- *Vehicle Management:* Supports regular and emergency vehicles with dynamic routing using Dijkstra's and A* algorithms.
- *Real-Time Simulation:* Updates traffic conditions every second, reflecting changes in vehicle positions and signal states.
- *Road Blockage Handling:* Ability to block roads during simulation, prompting vehicles to reroute if possible.
- *Priority Handling:* Emergency vehicles are given higher priority in routing and traffic signal management.
- *Interactive Dashboard:* Command-line interface to interact with the simulation, display statuses, and manage traffic scenarios.

## Installation

### Prerequisites

- *C++ Compiler:* Ensure you have g++ installed. You can install it using:
  - *Ubuntu/Debian:*
    bash
    sudo apt-get update
    sudo apt-get install build-essential
    
  - *MacOS (with Homebrew):*
    bash
    brew install gcc
    
  - *Windows:*
    - Install [MinGW](http://www.mingw.org/) or use [WSL](https://docs.microsoft.com/en-us/windows/wsl/install) for a Linux environment.

### Clone the Repository

```bash
git clone https://github.com/yourusername/traffic-management-simulation.git
cd traffic-management-simulation
```
