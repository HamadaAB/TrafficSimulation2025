#include "TrafficSimulation.h"



//===== Loading XML Data =====
/**
 * Reads XML elements to create roads, traffic lights, cars, and car factories
 * @param element - The XML element to process (like <BAAN>, <VERKEERSLICHT>, etc.)
 */

TrafficSimulation::TrafficSimulation(const std::string &doc_name) {
    TiXmlDocument doc(doc_name.c_str());
    if (!doc.LoadFile()) {
        // error message
        // std::cerr << "ERROR: UNABLE TO LOAD FILE" << '\n';
        // !!! return error file not found
    }
    TiXmlElement* root = doc.RootElement();
    std::string value_root = root->Value();
    if (value_root == "SIMULATION") {
        for (TiXmlElement *elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
            LoadElement(elem);
            if (!error_reason.empty()) {
                return;
            }
        }

    }
    else { LoadElement(root); }
    connect_to_road();
    sort_roads();

    // checks should be done here, after everything is loaded
    // otherwise it may be that a car can't be placed before the road
}

// This is like reading the game instruction manual
int TrafficSimulation::LoadElement(TiXmlElement* element) {
    if (!element) {
        // debug
        // std::cout << "Empty element found - skipping" << std::endl;
        return 0;
    }

    std::string elementType = element->Value();
    // debug only
    // std::cout << "Loading element: " << elementType << std::endl;

    // Load road information
    if (elementType == "BAAN") {
        Road newRoad;
        TiXmlElement* nameElem = element->FirstChildElement("naam");
        TiXmlElement* lengthElem = element->FirstChildElement("lengte");

        // Check for required data
        if (!nameElem or !lengthElem) {
            // std::cerr << "BAD ROAD: Missing name or length" << std::endl;
            error_reason += "MISSING_ELEM_BAAN";
            return 2;
        }

        try {
            int length = std::stoi(lengthElem->GetText());

            // the length of the rooad must be positive
            if (length <= 0) {
                // std::cerr << "BAD ROAD LENGTH: " << newRoad.get_length() << std::endl;
                error_reason += "BAD_LENGTH_BAAN";
                return 3;
            }
            newRoad.set_name(nameElem->GetText());
            newRoad.set_length(length);

            roads.push_back(newRoad); // Add road to global list
        }
        catch (...) {
            error_reason += "INVALID_BAAN_FORMAT";
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
            // std::cerr << "BAD TRAFFIC LIGHT: Missing data" << std::endl;
            // error message !!!
            return 2;
        }

        try {
            std::string roadName = roadElem->GetText();
            newLight.set_road(get_road(roadName));
            newLight.set_position(std::stoi(posElem->GetText()));
            newLight.set_cyclus(std::stoi(cycleElem->GetText()));

            // Check if there is a road
            Road* road = newLight.get_road();

            if (road == nullptr) {
                newLight.set_temp_name(roadName);
                // std::cerr << "TRAFFIC LIGHT ROAD NOT FOUND: " << newLight.get_road_name() << std::endl;
                // error message !!!
                return 4;
            }
            road->add_light(&newLight);

            trafficlights.push_back(newLight); // Add light to global list
        }
        catch (...) {
            // std::cerr << "INVALID TRAFFIC LIGHT NUMBER FORMAT" << std::endl;
            // error message !!!
        }
    }
    // Load vehicle
    else if (elementType == "VOERTUIG") {
        TiXmlElement* roadElem = element->FirstChildElement("baan");
        TiXmlElement* posElem = element->FirstChildElement("positie");
        TiXmlElement* typElem = element->FirstChildElement("type");

        // Check required elements exist
        if (!roadElem || !posElem) {
            // std::cerr << "BAD VEHICLE: Missing data" << std::endl;
            // error message !!!
            return 2;
        }
        // if exists: get the type of vehicle as a string
        std::string typ;
        if (!typElem) {
            typ = "autowagen";
        } else {
            typ = typElem->GetText();
        }

        try {
            std::string roadName = roadElem->GetText();
            int position = std::stoi(posElem->GetText());

            // Check if road actually exists
            Road* road = get_road(roadName);


            Vehicle new_car(road, position, typ);

            if (road == nullptr) {
                new_car.set_temp_name(roadName);
                // std::cerr << "VEHICLE ROAD NOT FOUND: " << roadName << std::endl;
                // error message !!!

            } else {
                vehicles.emplace_back(new_car);
                road->add_car(&vehicles.back());
            }


        }
        catch (...) {
            // std::cerr << "INVALID VEHICLE NUMBER FORMAT" << std::endl;
            // error message !!!
        }
    }
    //--- Load Car Factory ---
    else if (elementType == "VOERTUIGGENERATOR") {

        TiXmlElement* roadElem = element->FirstChildElement("baan");
        TiXmlElement* freqElem = element->FirstChildElement("frequentie");

        // Check required elements exist
        if (!roadElem || !freqElem) {
            // std::cerr << "BAD GENERATOR: Missing data" << std::endl;
            // error message !!!
            return 2;
        }

        try {
            std::string roadName = roadElem->GetText();
            Road* road = get_road(roadName);
            int frequency = std::stoi(freqElem->GetText());
            VehicleGenerator newGen(road, frequency);
            // Check for positive frequency
            if (frequency <= 0) {
                // std::cerr << "BAD GENERATOR FREQUENCY: " << newGen.frequency << std::endl;
                // error message !!!
                return 3;
            }

            // Check if road actually exists
            if (road == nullptr) {
                newGen.set_temp_name(roadName);
                // std::cerr << "GENERATOR ROAD NOT FOUND: " << newGen.road << std::endl;
                // error message !!!
                return 4;
            }

            vehicle_gens.push_back(newGen);

        }
        catch (...) {
            // std::cerr << "INVALID GENERATOR NUMBER FORMAT" << std::endl;
            // error message !!!
        }
    }
    // load busstop information
    else if (elementType == "BUSHALTE") {
        Bushalte newBushalte;
        TiXmlElement* roadElem = element->FirstChildElement("baan");
        TiXmlElement* posElem = element->FirstChildElement("positie");
        TiXmlElement* waitElem = element->FirstChildElement("wachttijd");

        // Check required elements exist
        if (!roadElem || !posElem || !waitElem) {
            // std::cerr << "BAD BUSSTOP: Missing data" << std::endl;
            // error message !!!
            return 2;
        }

        try {
            newBushalte.set_road_name(roadElem->GetText());
            newBushalte.set_position(std::stoi(posElem->GetText()));
            newBushalte.set_wait_time(std::stoi(waitElem->GetText()));

            // Check if there is a road
            bool roadExists = false;
            for (const Road& r : roads) {
                if (r.get_name() == newBushalte.get_road_name()) {
                    roadExists = true;
                    break;
                }
            }

            if (!roadExists) {
                // std::cerr << "BUSSTOP ROAD NOT FOUND: " << newBushalte.get_road_name() << std::endl;
                // error message !!!
                return 4;
            }

            bushalten.push_back(newBushalte); // Add busstop to global list
        }
        catch (...) {
            // std::cerr << "BUSSTOP LIGHT NUMBER FORMAT" << std::endl;
            // error message !!!
        }
    }
    // load busstop information
    else if (elementType == "KRUISPUNT") {
        Kruispunt newKruispunt;
        TiXmlElement* road1Elem = element->FirstChildElement("baan");

        // Check required elements exist
        if (!road1Elem) {
            // std::cerr << "BAD CROSSROAD: Missing data" << std::endl;
            // error message !!!
            return 2;
        }

        TiXmlElement* road2Elem = road1Elem->NextSiblingElement();
        // Check required elements exist
        if (!road2Elem) {
            // std::cerr << "BAD CROSSROAD: Missing data" << std::endl;
            // error message !!!
            return 2;
        }
        try {
            newKruispunt.set_road1_name(road1Elem->GetText());
            newKruispunt.set_road2_name(road2Elem->GetText());
            newKruispunt.set_position1(std::stoi(road1Elem->Attribute("positie")));
            newKruispunt.set_position2(std::stoi(road2Elem->Attribute("positie")));

            // Check if there is a road
            bool roadExists = false;
            bool road2Exists = false;
            for (const Road& r : roads) {
                if (r.get_name() == newKruispunt.get_road1_name()) {
                    roadExists = true;
                }
                if (r.get_name() == newKruispunt.get_road2_name()) {
                    road2Exists = true;
                }
                if (roadExists and road2Exists) {
                    break;
                }
            }

            if (!roadExists) {
                // std::cerr << "CROSSROAD ROAD NOT FOUND: " << newKruispunt.get_road1_name() << std::endl;
                // error message !!!
                return 4;
            }
            if (!road2Exists) {
                // std::cerr << "CROSSROAD ROAD NOT FOUND: " << newKruispunt.get_road2_name() << std::endl;
                // error message !!!
                return 4;
            }

            kruispunten.push_back(newKruispunt); // Add crossroad to global list
        }
        catch (...) {
            // std::cerr << "INVALID CROSSROAD NUMBER FORMAT" << std::endl;
            // error message !!!
        }
    }
        // error message !!!
    else { return 5; }
    return 0;
}

//##################### CAR MOVEMENT ###################################

/**
 * Updates car positions/speeds and handles traffic lights
 * @param dt - Time since last update (in seconds)
 * @param current_time - Total simulation time
 */

void TrafficSimulation::UpdateVehicleMovement(double dt, double current_time) {
    REQUIRE(current_time >= 0, "time can't be negative");
    REQUIRE(dt > 0, "time steps can't be negative or zero");
    // Update each vehicle
    sort_roads();
    for (Vehicle* car : ordered_vehicles) {
        car->update(dt, current_time);
    }
    for (Road& road : roads) {
        road.remove_offroad_cars();
    }

    remove_offroad_cars();
}

void TrafficSimulation::remove_offroad_cars() {
    std::list<Vehicle>::iterator it = vehicles.begin();
    while (it != vehicles.end()) {
        if (it->get_road() == nullptr) {
            it = vehicles.erase(it); // erase returns the next valid iterator
        } else {
            ++it;
        }
    }
}

// goes over all and updates them every time step
int TrafficSimulation::update_cycle(int repeatx) {

    double time = 0;
    bool cycle_on = true;
    while (cycle_on) {
        double new_time = time + 1;

        // update all
        UpdateTrafficLights(new_time);

        UpdateVehicleMovement(10, new_time);

        GenerateVehicles(new_time);

        time = new_time;

        if (vehicles.empty() or time == repeatx) { cycle_on = false; }
    }

    return 0;
}

//===== CAR SPAWNER =====

/**
 * Creates new cars at road starts based on generators
 * @param current_time - Total simulation time
 */

// Makes new car

void TrafficSimulation::GenerateVehicles(double current_time) {
    REQUIRE(current_time >= 0, "time can't be negative");
    for (VehicleGenerator& factory : vehicle_gens) {

        // !!! temp to compile
        if (factory.get_road() == nullptr) {
            return;
        }


        /*
        // Check if enough time has passed since last spawn
        if ((current_time - factory.last_generated) < factory.frequency) continue;

        // Debug message

        std::cout << "Generator check: "
                  << (current_time - factory.last_generated)
                  << "/" << factory.frequency << "s" << std::endl;


        if (!factory.get_road()) {
            std::cout << "Couldn't find road " << factory.get_road()->get_name() << std::endl;
            continue;
        }

        // Check space at road start
        bool spaceFree = true;
        double checkDistance = 2 * Constants::VehicleLength;

        // !!!
        // road.get_first_car()

        for (const Vehicle& car : vehicles) {
            if (car.get_road_name() == factory.road && car.get_position() < checkDistance) {
                spaceFree = false;
                // Debug?
                // std::cout << "Road start occupied" << std::endl;
                break;
            }
        }


        // Create new vehicle if space available



         * factory.road.add_car()
        if (spaceFree) {
            vehicles.emplace_back(factory.road, 0.0, 10.0, Constants::MaxAcceleration);
            factory.last_generated = current_time;
            // debug
            // std::cout << "New car spawned on " << factory.road << "!" << std::endl;
        }
         */
    }
}

/**
 * Calculates how much a car should speed up/slow down
 * @param car - The vehicle to calculate for
 * @param effective_vmax - Current speed limit (changes near traffic lights)
 * @return Acceleration value
 */

//===== DEBUG TOOLS =====
/** Prints current simulation status */

/*
void TrafficSimulation::PrintSituation() {
    // gives the vehicle number and his values
    for (int i = 0; i<int (vehicles.size()); i++) {
        std::cout<<"Car" << i << std::endl;
        vehicles[i].print();
    }
}
*/

//===== TRAFFIC LIGHT UPDATER (Changing Light Colors) =====

/** Updates traffic light colors based on cycle time */

void TrafficSimulation::UpdateTrafficLights(double current_time) {
    REQUIRE(current_time >= 0, "time can't be negative");
    for (TrafficLight& tlight : trafficlights) {
        tlight.update(current_time);
    }
}


std::list<Road> TrafficSimulation::get_roads() { return roads; }
std::list<TrafficLight> TrafficSimulation::get_lights() { return trafficlights; }
std::list<Vehicle> TrafficSimulation::get_vehicles() { return vehicles; }
std::vector<VehicleGenerator> TrafficSimulation::get_generators() { return vehicle_gens; }
std::vector<Bushalte> TrafficSimulation::get_busstops() { return bushalten; }
std::vector<Kruispunt> TrafficSimulation::get_crossroads() { return kruispunten; }
std::string TrafficSimulation::get_error_reason() { return error_reason; }

Road* TrafficSimulation::get_road(const std::string& name) {
    REQUIRE(!name.empty(), "road name can't be empty");
    for (Road& road : roads) {
        if (road.get_name() == name) {
            return &road;
        }
    }
    return nullptr;
}

void TrafficSimulation::connect_to_road() {
    std::list<Vehicle>::iterator vit = vehicles.begin();
    while (vit != vehicles.end()) {
        if (vit->get_road() == nullptr) {
            vit->set_road(get_road(vit->get_temp_name()));
            vit->set_temp_name("");

            if (vit->get_road() == nullptr) {
                vit = vehicles.erase(vit);
            } else {
                Road* road = vit->get_road();
                road->add_car(&(*vit));
                ++vit;
            }
        } else {
            ++vit;
        }
    }

// Process traffic lights
    std::list<TrafficLight>::iterator lit = trafficlights.begin();
    while (lit != trafficlights.end()) {
        if (lit->get_road() == nullptr) {
            lit->set_road(get_road(lit->get_temp_name()));
            lit->set_temp_name("");

            if (lit->get_road() == nullptr) {
                lit = trafficlights.erase(lit);
            } else {
                lit->get_road()->add_light(&(*lit));
                ++lit;
            }
        } else {
            ++lit;
        }
    }

    for (unsigned int i = 0; i<vehicle_gens.size();) {
        if (vehicle_gens[i].get_road() == nullptr) {
            vehicle_gens[i].set_road(get_road(vehicle_gens[i].get_temp_name()));
            vehicle_gens[i].set_temp_name("");
            if (vehicle_gens[i].get_road() == nullptr) {
                vehicle_gens.erase(vehicle_gens.begin() + int(i));
            } else { i++; }
        } else { i++; }
    }
}

bool sort_cars(Vehicle* car1, Vehicle* car2) {
    return car1->get_position() < car2->get_position();
}

void  TrafficSimulation::sort_roads() {
    ordered_vehicles.clear();
    for (Vehicle& car : vehicles) {
        ordered_vehicles.emplace_back(&car);
    }
    std::sort(ordered_vehicles.begin(), ordered_vehicles.end(), sort_cars);
    for (Road& road : roads) {
        road.sort_all();
    }
}
