#include "TrafficLight.h"
#include "Road.h"

TrafficLight::TrafficLight() {
    last_change_time = 0;
    is_light_green = true;
    cyclus = 5;
}

Road* TrafficLight::get_road() const{ return road; }
std::string TrafficLight::get_temp_name() const { return temp_road_name; }
std::string TrafficLight::get_road_name() { return road->get_name(); }
int TrafficLight::get_position() const{ return position; }
int TrafficLight::get_cyclus() const{ return cyclus; }
bool TrafficLight::is_green() const{ return is_light_green; }
double TrafficLight::get_last_change_time() const{ return last_change_time; }

void TrafficLight::set_temp_name(const std::string& name) { temp_road_name = name; }
void TrafficLight::set_road(Road* road_ptr) {
    REQUIRE(road_ptr != nullptr, "road can't be a nullptr");
    road = road_ptr;
}
void TrafficLight::set_position(int pos) {
    REQUIRE(pos >= 0, "position can't be negative");
    position = pos;
}
void TrafficLight::set_cyclus(int new_cyclus) {
    REQUIRE(new_cyclus >= 0, "cyclus can't be negative");
    cyclus = new_cyclus;
}
void TrafficLight::set_light(bool light) { is_light_green = light; }
void TrafficLight::set_last_change_time(double time) {
    REQUIRE(time >= 0, "time can't be negative");
    last_change_time = time;
}

void TrafficLight::change_light() {
    if (is_green()) {
        is_light_green = false;
    }
    else {
        is_light_green = true;
    }
}

void TrafficLight::update(double current_time) {
    REQUIRE(current_time >= 0, "current time can't be negative");
    double cycleTime = get_cyclus() * Constants::SimulationTimeStep;  // Convert to seconds!
    if (current_time - get_last_change_time() >= cycleTime) {
        change_light();
        last_change_time = current_time;
    }
}