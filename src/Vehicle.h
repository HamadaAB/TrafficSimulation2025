#ifndef VEHICLE_H
#define VEHICLE_H

#include "string"
#include "Constants.h"

class Vehicle {
public:
    std::string road_name;
    double position;
    double speed = 10.0;     // Set an initial speed
    double acceleration  = 1.0; // Set a small initial acceleration
    double length = Constants::VehicleLength;
    

    // Or a constructor that allows setting values when creating a vehicle
    Vehicle(const std::string& road, double pos,
            double spd = 10.0,
            double acc = Constants::MaxAcceleration)
        : road_name(road), position(pos), speed(spd), acceleration(acc) {}
};

#endif //VEHICLE_H