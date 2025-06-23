#ifndef VEHICLE_H
#define VEHICLE_H

#include <utility>

#include "string"
#include "Constants.h"
#include "iostream"
#include <cmath>
#include "TrafficLight.h"

// auto is een keyword en dus is het makkelijker om een andere naam te kiezen
enum Type {autowagen, bus, brandweerwagen, ziekenwagen, politiecombi};


class Vehicle {
private:
    Road* road;
    std::string temp_road_name;
    bool offroad = false;
    double position;
    double speed;
    double acceleration;
    double length = 4.0;
    Type type = autowagen;  // default

    Vehicle* get_next_car();
    TrafficLight* get_next_light();


public:
    // Or a constructor that allows setting values when creating a vehicle
    Vehicle(Road* road_ptr, double pos,
            double spd = 10.0,
            double acc = Constants::MaxAcceleration);

    Vehicle(Road* road_ptr, double pos, std::string& typ,
            double spd = 10.0, double acc = Constants::MaxAcceleration);

    void ComputeAcceleration(double effective_vmax);
    void update(double dt, double current_time);

    // Getters
    [[nodiscard]] Road* get_road() const;
    [[nodiscard]] std::string get_temp_name() const;
    [[nodiscard]] std::string get_road_name() const;
    [[nodiscard]] double get_position() const;
    [[nodiscard]] double get_speed() const;
    [[nodiscard]] double get_acceleration() const;
    [[nodiscard]] double get_length() const;

    // Setters
    void set_temp_name(const std::string& name);
    void set_road(Road* road_ptr);
    void set_position(double pos);
    void set_speed(double spd);
    void set_acceleration(double acc);
    void set_length(double len);
    void set_offroad();

    void add_speed(double spd);
    void move_position(double pos);

    void print();
};

Type string_to_car_type(const std::string& typ);

#endif //VEHICLE_H