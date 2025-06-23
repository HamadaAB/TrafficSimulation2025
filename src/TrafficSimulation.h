

#ifndef TRAFFICSIMULATION_H
#define TRAFFICSIMULATION_H


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
    // lists ?
    std::vector<Road> roads;
    std::vector<TrafficLight> trafficlights;
    std::vector<Vehicle> vehicles;
    std::vector<VehicleGenerator> vehicle_gens;
    std::vector<Bushalte> bushalten;
    std::vector<Kruispunt> kruispunten;

public:
    explicit TrafficSimulation(const std::string& doc_name);
    int LoadElement(TiXmlElement* element);

    std::vector<Road> get_roads();
    Road* get_road(const std::string& name);
    std::vector<TrafficLight> get_lights();
    std::vector<Vehicle> get_vehicles();
    std::vector<VehicleGenerator> get_generators();
    std::vector<Bushalte> get_busstops();
    std::vector<Kruispunt> get_crossroads();


    void PrintSituation();

    void UpdateVehicleMovement(double dt, double current_time);
    void remove_offroad_cars();
    void sort_roads();

    void UpdateTrafficLights(double current_time);
    void GenerateVehicles(double current_time);

    int update_cycle(int repeatx=-1);

    void connect_to_road();

};





#endif //TRAFFICSIMULATION_H
