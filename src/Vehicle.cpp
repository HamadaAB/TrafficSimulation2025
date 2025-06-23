#include "Vehicle.h"
#include "Road.h"

// normal constructor
Vehicle::Vehicle(Road*  road_ptr, double pos, double spd, double acc) {
    REQUIRE(pos >= 0, "position must be positive");
    REQUIRE(spd >= 0, "speed shouldn't be negative");
    road = road_ptr;
    position = pos;
    speed = spd;
    acceleration = acc;
}
// constructor with type
Vehicle::Vehicle(Road* road_ptr, double pos, std::string& typ, double spd, double acc)
                : Vehicle(road_ptr, pos, spd, acc) {
    type = string_to_car_type(typ);
}

Road* Vehicle::get_road() const { return road; }
std::string Vehicle::get_temp_name() const { return temp_road_name; }
std::string Vehicle::get_road_name() const { return road->get_name(); }
double Vehicle::get_position() const {
    ENSURE(position >= 0, "position can't be negative");
    return position;
}
double Vehicle::get_speed() const {
    ENSURE(speed >= 0, "speed can't be negative");
    return speed;
}
double Vehicle::get_acceleration() const { return acceleration; }
double Vehicle::get_length() const {
    ENSURE(length >= 0, "length can't be negative");
    return length;
}

// Setters
void Vehicle::set_temp_name(const std::string& name) { temp_road_name = name; }
void Vehicle::set_road(Road* road_ptr) {
    road = road_ptr; }
void Vehicle::set_position(double pos) {
    REQUIRE(pos >= 0, "position can't be negative");
    position = pos;
}
void Vehicle::set_speed(double spd) {
    REQUIRE(spd >= 0, "speed can't be negative");
    speed = spd;}
void Vehicle::set_acceleration(double acc) { acceleration = acc; }
void Vehicle::set_length(double len) {
    REQUIRE(len >= 0, "length can't be negative");
    length = len;
}
void Vehicle::set_offroad() { offroad = true; }

void Vehicle::add_speed(double spd) {speed += spd;}
void Vehicle::move_position(double pos) {position += pos;}

void Vehicle::print() {

    std::cout << "  Road: " << road->get_name() << "\n";
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

void Vehicle::ComputeAcceleration(double effective_vmax) {
    double gap = std::numeric_limits<double>::max(); // Start with maximum possible value

    // Find the closest car ahead
    Vehicle* frontCar = get_next_car();

    // Calculate following distance math between 2 cars
    double delta = 0.0;
    if (frontCar != nullptr) {

        double speedDiff = speed - frontCar->get_speed();
        if (gap < Constants::DecelerationDistance) {
            // Debug
            // std::cout << "Gap: " << gap<< ", SpeedDiff: " << speedDiff<< ", delta: " << delta << std::endl; // Debug
            delta = (Constants::MinFollowingDistance +
                     std::max(0.0, (speed * speedDiff) /
                                   (2 * std::sqrt(Constants::MaxAcceleration * Constants::MaxDeceleration)))) / gap;
        }
    }

    // Calculate acceleration using formula from specification B.3
    acceleration = Constants::MaxAcceleration * (1 - pow(speed/effective_vmax, 4) - delta*delta);
}

void Vehicle::update(double dt, double current_time) {
    REQUIRE(dt > 0, "time pass can't be negative or zero");
    REQUIRE(current_time >= 0, "current time can't be negative");

    // Traffic light logic
    bool shouldStop = false;
    double effectiveSpeed = Constants::MaxAbsoluteSpeed;

    TrafficLight* light = get_next_light();
    bool is_green = true;
    if (light != nullptr) {
        is_green = light->is_green();
    }
    if (!is_green and type != brandweerwagen and type != politiecombi and type != ziekenwagen) {
        double distanceToLight = light->get_position() - position;
        if (distanceToLight <= Constants::StopDistance) {
            shouldStop = true;
        } else if (distanceToLight <= Constants::DecelerationDistance) {
            effectiveSpeed = Constants::SlowdownFactor * Constants::MaxAbsoluteSpeed;
            // debug
            // std::cout << "Car at " << vehicle.get_position() << "m slowing for light ahead" << std::endl;
        }
    }
    ComputeAcceleration(effectiveSpeed);

    // Update position and speed
    if (shouldStop) {

        acceleration = -Constants::MaxDeceleration;

        if (speed + acceleration * dt < 0) {
            // We would go negative, so use formula B.2 for stopping completely
            move_position(-(speed * speed) / (2 * acceleration));
            speed = 0;
        } else {

            add_speed(get_acceleration() * dt);
            move_position(speed * dt + 0.5 * acceleration * dt * dt);
        }
    }
    else {
        // Normal movement using formula B.2
        if (speed + acceleration * dt < 0) {
            // Speed would go negative, so stop completely
            move_position(-(speed * speed) / (2 * acceleration));
            speed = 0;
        } else {
            // Update speed
            add_speed(acceleration * dt);
            // Limit speed to effective max speed
            if (speed > effectiveSpeed) {
                set_speed(effectiveSpeed);
            }
            // Update position
            move_position(speed * dt + 0.5 * acceleration * dt * dt);
        }
    }
}


Vehicle* Vehicle::get_next_car() {
    return road->get_next_car(position);
}

TrafficLight* Vehicle::get_next_light() {
    REQUIRE(road != nullptr, "road can't be nullptr");
    return road->get_next_light(position);
}