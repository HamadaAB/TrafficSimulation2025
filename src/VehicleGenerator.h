

#ifndef VEHICLEGENERATOR_H
#define VEHICLEGENERATOR_H

#include "string"


// This is like a car factory that creates new cars at set times
class VehicleGenerator {
public:
    std::string road;        // Which road to spawn on
    int frequency;          // How often to make new cars
    double last_generated;  // When we last made a car

    // Constructor sets up default values
    VehicleGenerator() : last_generated(0.0) {}
};


#endif //VEHICLEGENERATOR_H
