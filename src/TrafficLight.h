

#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "string"

class TrafficLight {
private:
    std::string road_name; // Name of the road the light is on
    int position;          // Position of the light on the road
    int cyclus;            // Cycle time for the light (how often it switches)
    bool is_light_green;         // State of the light: true = green, false = red
    double last_change_time; // Time when the light last changed state

public:
    [[nodiscard]] std::string get_road_name() const{return road_name;}
    [[nodiscard]] int get_position() const{return position;}
    [[nodiscard]] int get_cyclus() const{return cyclus;}
    [[nodiscard]] bool is_green() const{return is_light_green;}
    [[nodiscard]] double get_last_change_time() const{return last_change_time;}

    void set_road_name(const std::string& name) {road_name = name;}
    void set_position(int pos) {position = pos;}
    void set_cyclus(int new_cyclus) {cyclus = new_cyclus;}
    void change_light();
    void set_light(bool light) {is_light_green = light;}
    void set_last_change_time(double time) {last_change_time = time;}

    // Constructor to initialize the light
    TrafficLight() : is_light_green(true), last_change_time(0.0) {}
};
#endif //TRAFFICLIGHT_H
