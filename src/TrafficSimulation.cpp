#include "TrafficSimulation.h"

std::vector<Road> roads;
std::vector<TrafficLight> trafficlights;
std::vector<Vehicle> vehicles;
std::vector<VehicleGenerator> vehicle_gens;


void LoadElement(TiXmlElement* element) {
    if (!element) {
        // !!!
        // laad error ofzo
        std::cout<<"empty"<<std::endl;
        return;
    }
    std::string elementnaam = element->Value();

    if (elementnaam == "BAAN") {
        Road road;
        TiXmlElement* naamElem = element->FirstChildElement("naam");
        TiXmlElement* lengteElem = element->FirstChildElement("lengte");

        if (naamElem and lengteElem) {
            road.name = naamElem->GetText();
            road.length = std::stoi(lengteElem->GetText());
            roads.push_back(road);
        }
        else {
            // error: onbekend type
        }
    }
    else if (elementnaam == "VERKEERSLICHT") {
        TrafficLight tlight;
        TiXmlElement* baanElem = element->FirstChildElement("baan");
        TiXmlElement* positieElem = element->FirstChildElement("positie");
        TiXmlElement* cyclusElem = element->FirstChildElement("cyclus");

        if (baanElem and positieElem and cyclusElem) {
            tlight.road_name = baanElem->GetText();
            tlight.position = std::stoi(positieElem->GetText());
            tlight.cyclus = std::stoi(cyclusElem->GetText());
            trafficlights.push_back(tlight);
        } else {
            // error: onbekend type
        }
    }
    else if (elementnaam == "VOERTUIG") {
        Vehicle vehicle;
        TiXmlElement* baanElem = element->FirstChildElement("baan");
        TiXmlElement* positieElem = element->FirstChildElement("positie");

        if (baanElem and positieElem) {
            vehicle.road_name = baanElem->GetText();
            vehicle.position = std::stoi(positieElem->GetText());
            vehicles.push_back(vehicle);
        } else {
            // error: onvoldoende elementen
        }
    }
    else if (elementnaam == "VOERTUIGGENERATOR") {
        VehicleGenerator vehicle_gen;
        TiXmlElement* baanElem = element->FirstChildElement("baan");
        TiXmlElement* positieElem = element->FirstChildElement("positie");

        if (baanElem and positieElem) {
            vehicle_gen.road = baanElem->GetText();
            vehicle_gen.frequency = std::stoi(positieElem->GetText());
            vehicle_gens.push_back(vehicle_gen);
        } else {
            // error: onvoldoende elementen
        }
    }
    else {
        // error: onbekend type
        // misschien zien of kind een bekend element is en over de kinderen gaan
    }
}


void PrintSituation() {
    // !!!
    // cout of time must go here

    // gives the vehicle number and his values
    // geeft het voertuig nummer en zijn gegevens
    for (int i = 0; i<vehicles.size(); i++) {
        std::cout<<"Voertuig " << i << std::endl;
        PrintVehicleInf(vehicles[i]);
    }
}
// can be put into the above, this might make it easier to read,
// but it is used in a debug function
void PrintVehicleInf(const Vehicle& vehicle) {
    std::cout<< "baan: "<< vehicle.road_name<< std::endl;
    std::cout<< "positie: "<< vehicle.position<< std::endl;
    std::cout<< "snelheid: "<< vehicle.speed<< std::endl;
    std::cout<< std::endl;
}

// handy for debugging
void PrintAll() {
    for (const Vehicle& vehicle : vehicles) {
        PrintVehicleInf(vehicle);
        std::cout<< std::endl;
    }
    for (const Road& road : roads) {
        std::cout<< road.name << std::endl;
        std::cout<< road.length << std::endl;
        std::cout<< std::endl;
    }
    for (const TrafficLight& tlight : trafficlights) {
        std::cout<< tlight.road_name << std::endl;
        std::cout<< tlight.position << std::endl;
        std::cout<< std::endl;
    }

    std::cout<< std::endl;
}