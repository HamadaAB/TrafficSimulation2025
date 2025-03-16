

#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "string"

class TrafficLight {
public:
    std::string road_name; // Name of the road the light is on
    int position;          // Position of the light on the road
    int cyclus;            // Cycle time for the light (how often it switches)
    bool is_green;         // State of the light: true = green, false = red
    double last_change_time; // Time when the light last changed state

    // Constructor to initialize the light
    TrafficLight() : is_green(true), last_change_time(0.0) {}
};
#endif //TRAFFICLIGHT_H
