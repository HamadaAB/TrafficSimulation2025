

#ifndef TRAFFICSIMULATION_H
#define TRAFFICSIMULATION_H

// This is like the table of contents for our traffic simulation.
// It lists all the main functions we'll use to make cars go vroom!

#include "tinyxml/tinyxml.h"
#include "vector"
#include "string"
#include "iostream"
#include "Road.h"
#include "TrafficLight.h"
#include "Vehicle.h"
#include "VehicleGenerator.h"
#include "Bushalte.h"
#include "Constants.h"
#include <cmath>
#include "Kruispunt.h"
class TrafficSimulation {
private:
    std::vector<Road> roads;
    std::vector<TrafficLight> trafficlights;
    std::vector<Vehicle> vehicles;
    std::vector<VehicleGenerator> vehicle_gens;
    std::vector<Bushalte> bushalten;
    std::vector<Kruispunt> kruispunten;

public:
    explicit TrafficSimulation(const std::string& doc_name);
    int LoadElement(TiXmlElement* element); // Reads XML elements

    void PrintSituation(); // Shows current traffic status
// split for less duplicate code, used in debug function

    void UpdateVehicleMovement(double dt, double current_time);// Moves cars
    double ComputeAcceleration(const Vehicle& vehicle, double effective_vmax); // Calculates car acceleration

    void UpdateTrafficLights(double current_time);
    void GenerateVehicles(double current_time);

    int update_cycle();

};





#endif //TRAFFICSIMULATION_H
