cmake_minimum_required(VERSION 3.30)
project(TRaffaicProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(TRaffaicProject main.cpp
        Road.cpp
        Roa.h
        TrafficLight.cpp
        TrafficLight.h
        TrafficSimulation.cpp
        TrafficSimulation.h
        Vehicle.cpp
        Vehicle.h
        VehicleGenerator.cpp
        VehicleGenerator.h)
