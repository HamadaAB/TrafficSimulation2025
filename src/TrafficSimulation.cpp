


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
            newRoad.name = nameElem->GetText();
            newRoad.length = std::stoi(lengthElem->GetText());

            // the length of the rooad must be positive
            if (newRoad.length <= 0) {
                std::cerr << "BAD ROAD LENGTH: " << newRoad.length << std::endl;
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
            newLight.road_name = roadElem->GetText();
            newLight.position = std::stoi(posElem->GetText());
            newLight.cyclus = std::stoi(cycleElem->GetText());

            // Check if there is a road
            bool roadExists = false;
            for (const Road& r : roads) {
                if (r.name == newLight.road_name) {
                    roadExists = true;
                    break;
                }
            }

            if (!roadExists) {
                std::cerr << "TRAFFIC LIGHT ROAD NOT FOUND: " << newLight.road_name << std::endl;
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

        // Check required elements exist
        if (!roadElem || !posElem) {
            std::cerr << "BAD VEHICLE: Missing data" << std::endl;
            return;
        }

        try {
            std::string roadName = roadElem->GetText();
            int position = std::stoi(posElem->GetText());

            // Check if road actually exists
            bool roadExists = false;
            for (const Road& r : roads) {
                if (r.name == roadName) {
                    roadExists = true;
                    break;
                }
            }

            if (!roadExists) {
                std::cerr << "VEHICLE ROAD NOT FOUND: " << roadName << std::endl;
                return;
            }

            vehicles.emplace_back(roadName, position);
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
                if (r.name == newGen.road) {
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
        double cycleSeconds = light.cyclus * Constants::SimulationTimeStep;
        if (current_time - light.last_change_time >= cycleSeconds) {

            light.is_green = !light.is_green;
            light.last_change_time = current_time;
            std::cout << "Light at " << light.position << "m changed to "
                      << (light.is_green ? "GREEN" : "RED") << std::endl; // Debug
        }
    }

    // Update each vehicle
    for (Vehicle& vehicle : vehicles) {
        // Find the road the vehicle is on
        Road* currentRoad = nullptr;
        for (Road& r : roads) {
            if (r.name == vehicle.road_name) {
                currentRoad = &r;
                break;
            }
        }
        if (!currentRoad) continue; // Skip if road not found

        // Traffic light logic
        bool shouldStop = false;
        double effectiveSpeed = Constants::MaxAbsoluteSpeed;

        for (const TrafficLight& light : trafficlights) {
            if (light.road_name == vehicle.road_name) {
                double distanceToLight = light.position - vehicle.position;

                // Only respond to lights ahead of the vehicle
                if (distanceToLight > 0 && !light.is_green) {
                    // Red light logic
                    if (distanceToLight <= Constants::StopDistance) {
                        shouldStop = true;

                        break;  // Stop checking other lights
                    }
                    else if (distanceToLight <= Constants::DecelerationDistance) {
                        effectiveSpeed = Constants::SlowdownFactor * Constants::MaxAbsoluteSpeed;
                        std::cout << "Car at " << vehicle.position << "m slowing for light ahead" << std::endl;
                    }
                }
            }
        }


            vehicle.acceleration = ComputeAcceleration(vehicle, effectiveSpeed);


        // Update position and speed
        if (shouldStop) {



            vehicle.acceleration = -Constants::MaxDeceleration;

            if (vehicle.speed + vehicle.acceleration * dt < 0) {
                // We would go negative, so use formula B.2 for stopping completely
                vehicle.position -= (vehicle.speed * vehicle.speed) / (2 * vehicle.acceleration);
                vehicle.speed = 0;
            } else {

                vehicle.speed += vehicle.acceleration * dt;
                vehicle.position += vehicle.speed * dt + 0.5 * vehicle.acceleration * dt * dt;
            }
        } else {
            // Normal movement using formula B.2
            if (vehicle.speed + vehicle.acceleration * dt < 0) {
                // Speed would go negative, so stop completely
                vehicle.position -= (vehicle.speed * vehicle.speed) / (2 * vehicle.acceleration);
                vehicle.speed = 0;
            } else {
                // Update speed
                vehicle.speed += vehicle.acceleration * dt;
                // Limit speed to effective max speed
                if (vehicle.speed > effectiveSpeed) {
                    vehicle.speed = effectiveSpeed;
                }
                // Update position
                vehicle.position += vehicle.speed * dt + 0.5 * vehicle.acceleration * dt * dt;
            }
        }

        // Remove vehicles that exit the road
        if (vehicle.position >= currentRoad->length - Constants::VehicleLength) {
            std::cout << "Vehicle exited " << vehicle.road_name
                      << " at " << vehicle.position << "m" << std::endl;
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
            if (r.name == factory.road) {
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
            if (car.road_name == factory.road && car.position < checkDistance) {
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
        if (other.road_name == car.road_name && other.position > car.position) {

            double distance = other.position - car.position - Constants::VehicleLength; // Add this
            if (distance < gap) {
                gap = distance;
                frontCar = const_cast<Vehicle*>(&other);
            }
        }
    }

    // Calculate following distance math between 2 cars
    double delta = 0.0;
    if (frontCar) {

        double speedDiff = car.speed - frontCar->speed;
        if (gap < Constants::DecelerationDistance) {
            std::cout << "Gap: " << gap<< ", SpeedDiff: " << speedDiff<< ", delta: " << delta << std::endl; // Debug
            delta = (Constants::MinFollowingDistance +
                    std::max(0.0, (car.speed * speedDiff) /
                    (2 * std::sqrt(Constants::MaxAcceleration * Constants::MaxDeceleration)))) / gap;
        }
    }

    // Calculate acceleration using formula from specification B.3
    double acceleration = Constants::MaxAcceleration *
                         (1 - pow(car.speed/effective_vmax, 4) - delta*delta);



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
    std::cout << "  Road: " << car.road_name << "\n";
    std::cout << "  Position: " << car.position << " m\n";
    std::cout << "  Speed: " << car.speed << " m/s\n";
}



//===== TRAFFIC LIGHT UPDATER (Changing Light Colors) =====

/** Updates traffic light colors based on cycle time */

void UpdateTrafficLights(double current_time) {
    for (TrafficLight& tlight : trafficlights) {
        double cycleTime = tlight.cyclus * Constants::SimulationTimeStep;  // Convert to seconds!
        if (current_time - tlight.last_change_time >= cycleTime) {
            tlight.is_green = !tlight.is_green;
            tlight.last_change_time = current_time;
        }
    }
}






