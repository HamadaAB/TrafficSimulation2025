#include "Road.h"
#include "Vehicle.h"
#include "TrafficLight.h"


std::string Road::get_name() const { return name; }
int Road::get_length() const {
    ENSURE(length >= 0, "length can't be negative");
    return length; }
std::vector<Vehicle*> Road::get_cars() const { return on_road; }

void Road::set_name(std::string new_name) {name = std::move(new_name);}
void Road::set_length(int new_length) {
    REQUIRE(new_length >= 0, "length can't be negative");
    length = new_length;
}

bool by_distance(const Vehicle* car1, const Vehicle* car2) {
    return car1->get_position() < car2->get_position();
}
bool by_distance_light(const TrafficLight* light1, const TrafficLight* light2) {
    return light1->get_position() < light2->get_position();
}


void Road::add_car(Vehicle* car) {
    REQUIRE(car != nullptr, "can't add a nullptr as a car");
    on_road.emplace_back(car);
    sort_cars();
}

void Road::add_light(TrafficLight* light) {
    REQUIRE(light != nullptr, "can't add a nullptr as a light");
    lights.emplace_back(light);
    sort_lights();
}

void Road::remove_offroad_cars() {
    for (int i = int(on_road.size()-1); 0<=i;) {
        if (length < on_road[i]->get_position()) {
            on_road[i]->set_road(nullptr);
            on_road.erase(on_road.begin() + i);
            i--;
        }
        else { break; }
    }
}

Vehicle* Road::get_next_car(double position) {
    REQUIRE(position>=0, "position can't be negative");
    if (on_road.empty() or on_road.size() >= 4000000000) { return nullptr; }
    for (Vehicle* car : on_road) {
        if (car->get_position() > position) {
            return car;
        }
    }
    return nullptr;
}

TrafficLight* Road::get_next_light(double position) {
    REQUIRE(position>=0, "position can't be negative");
    if (lights.empty() or lights.size() > 4289000000) {return nullptr;}
    for (TrafficLight* light : lights) {
        if (light->get_position() >= position) {
            return light;
        }
    }
    return nullptr;
}

void Road::update_cars(double dt, double current_time) {
    REQUIRE(dt>=0, "time pass can't be negative");
    REQUIRE(current_time>=0, "current time can't be negative");
    for (int i = int(on_road.size()-1); 0<=i;) {
        on_road[i]->update(dt, current_time);
        i--;
    }
}


void Road::sort_cars() {
    std::sort(on_road.begin(), on_road.end(), by_distance);
}

void Road::sort_lights() {
    std::sort(lights.begin(), lights.end(), by_distance_light);
}

void Road::sort_all() {
    sort_cars();
    sort_cars();
}
