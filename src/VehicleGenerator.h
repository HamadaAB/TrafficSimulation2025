

#ifndef VEHICLEGENERATOR_H
#define VEHICLEGENERATOR_H

#include "string"
#include "Road.h"
#include "Vehicle.h"


// This is like a car factory that creates new cars at set times
class VehicleGenerator {
private:
    Road* road;        // Which road to spawn on
    std::string temp_road_name;
    int frequency;          // How often to make new cars
    double last_generated;  // When we last made a car

public:
    // Constructor sets up default values
    VehicleGenerator();
    VehicleGenerator(Road* road_ptr, int frequency = 10, double last_generated = 0);

    std::string get_temp_name() const;

    void set_temp_name(const std::string& name);
    void set_road(Road* road_ptr);

    Road* get_road();
    [[nodiscard]] int get_frequency() const;
    [[nodiscard]] double get_last_generated() const;
};


#endif //VEHICLEGENERATOR_H
