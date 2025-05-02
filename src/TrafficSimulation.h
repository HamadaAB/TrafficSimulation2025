

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

void LoadDoc(std::string doc_name);
void LoadElement(TiXmlElement* element); // Reads XML elements

void PrintSituation(); // Shows current traffic status
// split for less duplicate code, used in debug function
void PrintVehicleInf(const Vehicle& vehicle); // Shows car details

void UpdateVehicleMovement(double dt, double current_time);// Moves cars
double ComputeAcceleration(const Vehicle& vehicle, double effective_vmax); // Calculates car acceleration

void UpdateTrafficLights(double current_time);
void GenerateVehicles(double current_time);



extern std::vector<Road> roads;
extern std::vector<TrafficLight> trafficlights;
extern std::vector<Vehicle> vehicles;
extern std::vector<VehicleGenerator> vehicle_gens;
extern std::vector<Bushalte> bushalten;

#endif //TRAFFICSIMULATION_H
