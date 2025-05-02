cmake_minimum_required(VERSION 3.6)
project(PSE)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
# Remove the "-O2" flag if you have problems with the debugger.
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Set include dir
include_directories(./gtest/include ${CMAKE_SOURCE_DIR}/src/tinyxml)

# Set Library dir
link_directories(./gtest/lib)
include_directories(${CMAKE_SOURCE_DIR}/src/tinyxml)


# Set source files for RELEASE target
# Only add cpp files, no h files!
# Only add files with functionality here, no test files!
# Make sure the "main" function is in "main_release"!
set(RELEASE_SOURCE_FILES ./src/main.cpp
        src/tinyxml/tinystr.cpp
        src/tinyxml/tinyxml.cpp
        src/tinyxml/tinyxmlerror.cpp
        src/tinyxml/tinyxmlparser.cpp
        ./src/Road.cpp
        ./src/TrafficLight.cpp
        ./src/TrafficSimulation.cpp
        ./src/VehicleGenerator.cpp
        ./src/Vehicle.cpp
        ./src/Bushalte.cpp
        ./src/Kruispunt.cpp

)


# Set source files for DEBUG target
# Only add cpp files, no h files!
# Make sure the "main" function is in "main_debug"!
set(DEBUG_SOURCE_FILES ./src/main.cpp
        src/tinyxml/tinystr.cpp
        src/tinyxml/tinyxml.cpp
        src/tinyxml/tinyxmlerror.cpp
        src/tinyxml/tinyxmlparser.cpp
        ./gtest/lib/libgtest.a
        ./gtest/lib/libgtest_main.a
        ./src/Road.cpp
        ./src/TrafficLight.cpp
        ./src/TrafficSimulation.cpp
        ./src/VehicleGenerator.cpp
        ./src/Vehicle.cpp
        src/Constants.h
        ./src/Bushalte.cpp
        src/Kruispunt.cpp
)

# Create RELEASE target
add_executable(release_target ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(debug_target ${DEBUG_SOURCE_FILES})

# Link libraries
target_link_libraries(release_target gtest gtest_main pthread)
target_link_libraries(debug_target gtest gtest_main pthread)






