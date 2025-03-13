
#ifndef VEHICLE_H
#define VEHICLE_H

#include "string"

class Vehicle {
public:
    std::string road_name;
    int position;
    double speed = 0; // can also be called velocity
    double acceleration = 0;
};

#endif //VEHICLE_H
