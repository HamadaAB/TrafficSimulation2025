#include "VehicleGenerator.h"

VehicleGenerator::VehicleGenerator() {
    // default
    frequency = 10;
    last_generated = 0;
}

VehicleGenerator::VehicleGenerator(Road* road_ptr, int freq, double last_gen) {
    road = road_ptr;
    frequency = freq;
    last_generated = last_gen;
}

std::string VehicleGenerator::get_temp_name() const { return temp_road_name; }

void VehicleGenerator::set_temp_name(const std::string& name) { temp_road_name = name; }
void VehicleGenerator::set_road(Road *road_ptr) {
    REQUIRE(road_ptr != nullptr, "road can't be nullptr");
    road = road_ptr;
}

Road* VehicleGenerator::get_road() { return road; }
int VehicleGenerator::get_frequency() const {
    ENSURE(frequency >= 0, "frequency can't be negative");
    return frequency;
}
double VehicleGenerator::get_last_generated() const {
    ENSURE(last_generated >= 0, "last generated can't be negative");
    return last_generated;
}
