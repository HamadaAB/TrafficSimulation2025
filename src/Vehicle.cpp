#include "Vehicle.h"

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