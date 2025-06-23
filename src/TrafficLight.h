

#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include "string"
#include "Constants.h"

class Road;

class TrafficLight {
private:
    Road* road;
    std::string temp_road_name;
    int position;          // Position of the light on the road
    int cyclus;            // Cycle time for the light (how often it switches)
    bool is_light_green;         // State of the light: true = green, false = red
    double last_change_time; // Time when the light last changed state

public:
    // getters
    [[nodiscard]] Road* get_road() const;
    [[nodiscard]] std::string get_temp_name() const;
    [[nodiscard]] std::string get_road_name() ;
    [[nodiscard]] int get_position() const;
    [[nodiscard]] int get_cyclus() const;
    [[nodiscard]] bool is_green() const;
    [[nodiscard]] double get_last_change_time() const;
    // setters
    void set_temp_name(const std::string& name);
    void set_road(Road* road_ptr);
    void set_position(int pos);
    void set_cyclus(int new_cyclus);
    void set_light(bool light);
    void set_last_change_time(double time);
    void change_light();

    void update(double current_time);

    // Constructor to initialize the light
    TrafficLight();
};
#endif //TRAFFICLIGHT_H
