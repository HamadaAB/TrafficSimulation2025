#include "Vehicle.h"
// normal constructor
Vehicle::Vehicle(const std::string&  road, double pos, double spd, double acc) {
    road_name = road;
    position = pos;
    speed = spd;
    acceleration = acc;
}
// constructor with type
Vehicle::Vehicle(std::string&  road, double pos, std::string& typ, double spd, double acc)
                : Vehicle(road, pos, spd, acc) {
    type = string_to_car_type(typ);
}

std::string Vehicle::get_road_name() const { return road_name; }
double Vehicle::get_position() const { return position; }
double Vehicle::get_speed() const { return speed; }
double Vehicle::get_acceleration() const { return acceleration; }
double Vehicle::get_length() const { return length; }

// Setters
void Vehicle::set_roadName(const std::string& road) { road_name = road; }
void Vehicle::set_position(double pos) { position = pos; }
void Vehicle::set_speed(double spd) { speed = spd; }
void Vehicle::set_acceleration(double acc) { acceleration = acc; }
void Vehicle::set_length(double len) { length = len; }

void Vehicle::add_speed(double spd) {speed += spd;}
void Vehicle::move_position(double pos) {position += pos;}

void Vehicle::print() {
    std::cout << "  Road: " << road_name << "\n";
    std::cout << "  Position: " << position << " m\n";
    std::cout << "  Speed: " << speed << " m/s\n";
}

// converts string to the types of a car; autowagen, bus,...
Type string_to_car_type(const std::string& typ) {
    if(typ == "auto") { return autowagen; }
    else if(typ == "bus") { return bus; }
    else if(typ == "brandweerwagen") { return brandweerwagen; }
    else if(typ == "ziekenwagen") { return ziekenwagen; }
    else if(typ == "politiecombi") { return politiecombi; }
    return autowagen;
}