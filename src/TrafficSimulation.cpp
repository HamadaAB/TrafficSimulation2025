


#include "TrafficSimulation.h"
#include "Constants.h"
#include "TrafficLight.h"
#include <cmath>
#include <iostream>


//===== Global Variables =====
// These are like the game memory they remember all roads, cars, etc
// All roads, traffic lights, cars, and car factories in the simulation
std::vector<Road> roads;
std::vector<TrafficLight> trafficlights;
std::vector<Vehicle> vehicles;
std::vector<VehicleGenerator> vehicle_gens;





//===== Loading XML Data =====
/**
 * Reads XML elements to create roads, traffic lights, cars, and car factories
 * @param element - The XML element to process (like <BAAN>, <VERKEERSLICHT>, etc.)
 */
// This is like reading the game instruction manual

void LoadElement(TiXmlElement* element) {
    if (!element) {
        std::cout << "Empty element found - skipping" << std::endl;
        return;
    }

    std::string elementType = element->Value();
    std::cout << "Loading element: " << elementType << std::endl;

    // Handle root simulation element
    if (elementType == "SIMULATION") {
        for (TiXmlElement* child = element->FirstChildElement();
             child;
             child = child->NextSiblingElement()) {
            LoadElement(child);
        }
        return;
    }

    // Load road information
    if (elementType == "BAAN") {
        Road newRoad;
        TiXmlElement* nameElem = element->FirstChildElement("naam");
        TiXmlElement* lengthElem = element->FirstChildElement("lengte");

        // Check for required data
        if (!nameElem || !lengthElem) {
            std::cerr << "BAD ROAD: Missing name or length" << std::endl;
            return;
        }

        try {
            newRoad.set_name(nameElem->GetText());
            newRoad.set_length(std::stoi(lengthElem->GetText()));

            // the length of the rooad must be positive
            if (newRoad.get_length() <= 0) {
                std::cerr << "BAD ROAD LENGTH: " << newRoad.get_length() << std::endl;
                return;
            }

            roads.push_back(newRoad); // Add road to global list
        }
        catch (...) {
            std::cerr << "INVALID ROAD NUMBER FORMAT" << std::endl;
        }
    }
    // Load traffic light
    else if (elementType == "VERKEERSLICHT") {
        TrafficLight newLight;
        TiXmlElement* roadElem = element->FirstChildElement("baan");
        TiXmlElement* posElem = element->FirstChildElement("positie");
        TiXmlElement* cycleElem = element->FirstChildElement("cyclus");

        // Check required elements exist
        if (!roadElem || !posElem || !cycleElem) {
            std::cerr << "BAD TRAFFIC LIGHT: Missing data" << std::endl;
            return;
        }

        try {
            newLight.set_road_name(roadElem->GetText());
            newLight.set_position(std::stoi(posElem->GetText()));
            newLight.set_cyclus(std::stoi(cycleElem->GetText()));

            // Check if there is a road
            bool roadExists = false;
            for (const Road& r : roads) {
                if (r.get_name() == newLight.get_road_name()) {
                    roadExists = true;
                    break;
                }
            }

            if (!roadExists) {
                std::cerr << "TRAFFIC LIGHT ROAD NOT FOUND: " << newLight.get_road_name() << std::endl;
                return;
            }

            trafficlights.push_back(newLight); // Add light to global list
        }
        catch (...) {
            std::cerr << "INVALID TRAFFIC LIGHT NUMBER FORMAT" << std::endl;
        }
    }
    // Load vehicle
    else if (elementType == "VOERTUIG") {
        TiXmlElement* roadElem = element->FirstChildElement("baan");
        TiXmlElement* posElem = element->FirstChildElement("positie");
        TiXmlElement* typElem = element->FirstChildElement("type");

        // Check required elements exist
        if (!roadElem || !posElem) {
            std::cerr << "BAD VEHICLE: Missing data" << std::endl;
            return;
        }
        std::string typ;
        if (!typElem) {
            std::cerr << "NO VEHICLE TYPE" << std::endl;
            typ = "autowagen";
        } else {
            typ = typElem->GetText();
        }

        try {
            std::string roadName = roadElem->GetText();
            int position = std::stoi(posElem->GetText());

            // Check if road actually exists
            bool roadExists = false;
            for (const Road& r : roads) {
                if (r.get_name() == roadName) {
                    roadExists = true;
                    break;
                }
            }

            if (!roadExists) {
                std::cerr << "VEHICLE ROAD NOT FOUND: " << roadName << std::endl;
                return;
            }

            vehicles.emplace_back(roadName, position, typ);
        }
        catch (...) {
            std::cerr << "INVALID VEHICLE NUMBER FORMAT" << std::endl;
        }
    }

    //--- Load Car Factory ---
    else if (elementType == "VOERTUIGGENERATOR") {
        VehicleGenerator newGen;
        TiXmlElement* roadElem = element->FirstChildElement("baan");
        TiXmlElement* freqElem = element->FirstChildElement("frequentie");

        // Check required elements exist
        if (!roadElem || !freqElem) {
            std::cerr << "BAD GENERATOR: Missing data" << std::endl;
            return;
        }

        try {
            newGen.road = roadElem->GetText();
            newGen.frequency = std::stoi(freqElem->GetText());

            // Check for positive frequency
            if (newGen.frequency <= 0) {
                std::cerr << "BAD GENERATOR FREQUENCY: " << newGen.frequency << std::endl;
                return;
            }

            // Check if road actually exists
            bool roadExists = false;
            for (const Road& r : roads) {
                if (r.get_name() == newGen.road) {
                    roadExists = true;
                    break;
                }
            }

            if (!roadExists) {
                std::cerr << "GENERATOR ROAD NOT FOUND: " << newGen.road << std::endl;
                return;
            }

            vehicle_gens.push_back(newGen);
        }
        catch (...) {
            std::cerr << "INVALID GENERATOR NUMBER FORMAT" << std::endl;
        }
    }
}

//##################### CAR MOVEMENT ###################################

/**
 * Updates car positions/speeds and handles traffic lights
 * @param dt - Time since last update (in seconds)
 * @param current_time - Total simulation time
 */

void UpdateVehicleMovement(double dt, double current_time) {
    std::vector<Vehicle> updatedVehicles;

    // Update traffic lights first
    for (TrafficLight& light : trafficlights) {
        double cycleSeconds = light.get_cyclus() * Constants::SimulationTimeStep;
        if (current_time - light.get_last_change_time() >= cycleSeconds) {

            light.change_light();
            light.set_last_change_time(current_time);
            std::cout << "Light at " << light.get_position() << "m changed to "
                      << (light.is_green() ? "GREEN" : "RED") << std::endl; // Debug
        }
    }

    // Update each vehicle
    for (Vehicle& vehicle : vehicles) {
        // Find the road the vehicle is on
        Road* currentRoad = nullptr;
        for (Road& r : roads) {
            if (r.get_name() == vehicle.get_road_name()) {
                currentRoad = &r;
                break;
            }
        }
        if (!currentRoad) continue; // Skip if road not found

        // Traffic light logic
        bool shouldStop = false;
        double effectiveSpeed = Constants::MaxAbsoluteSpeed;

        for (const TrafficLight& light : trafficlights) {
            if (light.get_road_name() == vehicle.get_road_name()) {
                double distanceToLight = light.get_position() - vehicle.get_position();

                // Only respond to lights ahead of the vehicle
                if (distanceToLight > 0 && !light.is_green()) {
                    // Red light logic
                    if (distanceToLight <= Constants::StopDistance) {
                        shouldStop = true;

                        break;  // Stop checking other lights
                    }
                    else if (distanceToLight <= Constants::DecelerationDistance) {
                        effectiveSpeed = Constants::SlowdownFactor * Constants::MaxAbsoluteSpeed;
                        std::cout << "Car at " << vehicle.get_position() << "m slowing for light ahead" << std::endl;
                    }
                }
            }
        }


            vehicle.set_acceleration(ComputeAcceleration(vehicle, effectiveSpeed));


        // Update position and speed
        if (shouldStop) {



            vehicle.set_acceleration(-Constants::MaxDeceleration);

            if (vehicle.get_speed() + vehicle.get_acceleration() * dt < 0) {
                // We would go negative, so use formula B.2 for stopping completely
                vehicle.move_position(-(vehicle.get_speed() * vehicle.get_speed()) / (2 * vehicle.get_acceleration()));
                vehicle.set_speed(0);
            } else {

                vehicle.add_speed(vehicle.get_acceleration() * dt);
                vehicle.move_position(vehicle.get_speed() * dt + 0.5 * vehicle.get_acceleration() * dt * dt);
            }
        } else {
            // Normal movement using formula B.2
            if (vehicle.get_speed() + vehicle.get_acceleration() * dt < 0) {
                // Speed would go negative, so stop completely
                vehicle.move_position(-(vehicle.get_speed() * vehicle.get_speed()) / (2 * vehicle.get_acceleration()));
                vehicle.set_speed(0);
            } else {
                // Update speed
                vehicle.add_speed(vehicle.get_acceleration() * dt);
                // Limit speed to effective max speed
                if (vehicle.get_speed() > effectiveSpeed) {
                    vehicle.set_speed(effectiveSpeed);
                }
                // Update position
                vehicle.move_position(vehicle.get_speed() * dt + 0.5 * vehicle.get_acceleration() * dt * dt);
            }
        }

        // Remove vehicles that exit the road
        if (vehicle.get_position() >= currentRoad->get_length() - Constants::VehicleLength) {
            std::cout << "Vehicle exited " << vehicle.get_road_name()
                      << " at " << vehicle.get_position() << "m" << std::endl;
            continue; // Skip adding to updatedVehicles
        }
        updatedVehicles.push_back(vehicle);
    }

    vehicles = updatedVehicles;
}


//===== CAR SPAWNER =====

/**
 * Creates new cars at road starts based on generators
 * @param current_time - Total simulation time
 */

// Makes new car

void GenerateVehicles(double current_time) {
    for (VehicleGenerator& factory : vehicle_gens) {
        // Check if enough time has passed since last spawn
        if ((current_time - factory.last_generated) < factory.frequency) continue;

        // Debug message
        std::cout << "Generator check: "
                  << (current_time - factory.last_generated)
                  << "/" << factory.frequency << "s" << std::endl;

        // Find the target road
        Road* targetRoad = nullptr;
        for (Road& r : roads) {
            if (r.get_name() == factory.road) {
                targetRoad = &r;
                break;
            }
        }
        if (!targetRoad) {
            std::cout << "Couldn't find road " << factory.road << std::endl;
            continue;
        }

        // Check space at road start
        bool spaceFree = true;
        double checkDistance = 2 * Constants::VehicleLength;

        for (const Vehicle& car : vehicles) {
            if (car.get_road_name() == factory.road && car.get_position() < checkDistance) {
                spaceFree = false;
                std::cout << "Road start occupied" << std::endl;
                break;
            }
        }

        // Create new vehicle if space available
        if (spaceFree) {
            vehicles.emplace_back(factory.road, 0.0, 10.0, Constants::MaxAcceleration);
            factory.last_generated = current_time;
            std::cout << "New car spawned on " << factory.road << "!" << std::endl;
        }
    }
}

/**
 * Calculates how much a car should speed up/slow down
 * @param car - The vehicle to calculate for
 * @param effective_vmax - Current speed limit (changes near traffic lights)
 * @return Acceleration value
 */


double ComputeAcceleration(const Vehicle& car, double effective_vmax) {



    Vehicle* frontCar = nullptr;
    double gap = std::numeric_limits<double>::max(); // Start with maximum possible value

    // Find closest car ahead
    for (const Vehicle& other : vehicles) {
        if (other.get_road_name() == car.get_road_name() && other.get_position() > car.get_position()) {

            double distance = other.get_position() - car.get_position() - Constants::VehicleLength; // Add this
            if (distance < gap) {
                gap = distance;
                frontCar = const_cast<Vehicle*>(&other);
            }
        }
    }

    // Calculate following distance math between 2 cars
    double delta = 0.0;
    if (frontCar) {

        double speedDiff = car.get_speed() - frontCar->get_speed();
        if (gap < Constants::DecelerationDistance) {
            std::cout << "Gap: " << gap<< ", SpeedDiff: " << speedDiff<< ", delta: " << delta << std::endl; // Debug
            delta = (Constants::MinFollowingDistance +
                    std::max(0.0, (car.get_speed() * speedDiff) /
                    (2 * std::sqrt(Constants::MaxAcceleration * Constants::MaxDeceleration)))) / gap;
        }
    }

    // Calculate acceleration using formula from specification B.3
    double acceleration = Constants::MaxAcceleration *
                         (1 - pow(car.get_speed()/effective_vmax, 4) - delta*delta);



    return acceleration;


}



//===== DEBUG TOOLS =====
/** Prints current simulation status */

void PrintSituation() {

    // gives the vehicle number and his values
    for (int i = 0; i<int (vehicles.size()); i++) {
        std::cout<<"Car" << i << std::endl;
        PrintVehicleInf(vehicles[i]);
    }
}

//===== SINGLE CAR INFO =====
/** Shows details for one car */

void PrintVehicleInf(const Vehicle& car) {
    std::cout << "  Road: " << car.get_road_name() << "\n";
    std::cout << "  Position: " << car.get_position() << " m\n";
    std::cout << "  Speed: " << car.get_speed() << " m/s\n";
}



//===== TRAFFIC LIGHT UPDATER (Changing Light Colors) =====

/** Updates traffic light colors based on cycle time */

void UpdateTrafficLights(double current_time) {
    for (TrafficLight& tlight : trafficlights) {
        double cycleTime = tlight.get_cyclus() * Constants::SimulationTimeStep;  // Convert to seconds!
        if (current_time - tlight.get_last_change_time() >= cycleTime) {
            tlight.change_light();
            tlight.set_last_change_time(current_time);
        }
    }
}






