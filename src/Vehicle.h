#ifndef VEHICLE_H
#define VEHICLE_H

#include <utility>

#include "string"
#include "Constants.h"

class Vehicle {
private:
    std::string road_name;
    double position;
    double speed;
    double acceleration;
    double length = Constants::VehicleLength;
public:
    // Or a constructor that allows setting values when creating a vehicle
    Vehicle(const std::string  road, double pos,
            double spd = 10.0,
            double acc = Constants::MaxAcceleration)
            : road_name(road), position(pos), speed(spd), acceleration(acc) {}

    // Getters
    std::string get_road_name() const { return road_name; }
    double get_position() const { return position; }
    double get_speed() const { return speed; }
    double get_acceleration() const { return acceleration; }
    double get_length() const { return length; }

    // Setters
    void set_roadName(const std::string& road) { road_name = road; }
    void set_position(double pos) { position = pos; }
    void set_speed(double spd) { speed = spd; }
    void set_acceleration(double acc) { acceleration = acc; }
    void set_length(double len) { length = len; }

    void add_speed(double spd) {speed += spd;}
    void move_position(double pos) {position += pos;}
};


#endif //VEHICLE_H