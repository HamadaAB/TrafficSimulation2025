

#ifndef ROAD_H
#define ROAD_H
#include <utility>
#include "algorithm"
#include "string"
#include "vector"


#include "DesignByContract.h"

class Vehicle;
class TrafficLight;

class Road {
private:
    std::string name;
    int length;
    std::vector<Vehicle*> on_road;
    std::vector<TrafficLight*> lights;

public:
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] int get_length() const;
    [[nodiscard]] std::vector<Vehicle*> get_cars() const;

    void set_name(std::string new_name);
    void set_length(int new_length);

    void update_cars(double dt, double current_time);

    void add_car(Vehicle* car);
    void add_light(TrafficLight* light);

    void remove_offroad_cars();

    Vehicle* get_next_car(double position);

    TrafficLight* get_next_light(double position);

    void sort_cars();
    void sort_lights();
    void sort_all();
};
#endif //ROAD_H
