

#ifndef TRAFFICSIMULATION_H
#define TRAFFICSIMULATION_H

#include "vector"
#include "string"
#include "iostream"

#include "../tinyxml/tinyxml.h"

#include "Road.h"
#include "TrafficLight.h"
#include "Vehicle.h"
#include "VehicleGenerator.h"


void LoadElement(TiXmlElement* element);

void PrintSituation();
// split for less duplicate code, used in debug function
void PrintVehicleInf(const Vehicle& vehicle);


// gives all information; vehicles, roads and traffic lights
// handy for debugging, but not needed
void PrintAll();


#endif //TRAFFICSIMULATION_H
