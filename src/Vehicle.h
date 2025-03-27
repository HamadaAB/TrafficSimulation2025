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
    [[nodiscard]] std::string get_road_name() const;
    [[nodiscard]] double get_position() const;
    [[nodiscard]] double get_speed() const;
    [[nodiscard]] double get_acceleration() const;
    [[nodiscard]] double get_length() const;

    // Setters
    void set_roadName(const std::string& road);
    void set_position(double pos);
    void set_speed(double spd);
    void set_acceleration(double acc);
    void set_length(double len);

    void add_speed(double spd);
    void move_position(double pos);
};


#endif //VEHICLE_H