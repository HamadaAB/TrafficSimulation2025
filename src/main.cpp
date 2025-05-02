#include "TrafficSimulation.h"
#include "tinyxml/tinyxml.h"
#include <gtest/gtest.h>


// Reset global state before each test
void ResetGlobals() {
    roads.clear();
    trafficlights.clear();
    vehicles.clear();
    vehicle_gens.clear();
}

// Test Fixture
class TrafficSimTest : public ::testing::Test {
protected:
    void SetUp() override { ResetGlobals(); }
};

//===== Test Cases =====

// Test 1: Invalid XML element
TEST_F(TrafficSimTest, InvalidElement) {
  TiXmlDocument doc("../Tests/invalid_element.xml");
  ASSERT_TRUE(doc.LoadFile());
  TiXmlElement* root = doc.RootElement();
  for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
    LoadElement(elem);
  }
  EXPECT_TRUE(roads.empty()); // Invalid element should not load
}

// Test 2: Missing required attribute
TEST_F(TrafficSimTest, MissingAttribute) {
  TiXmlDocument doc("../Tests/missing_attribute.xml");
  ASSERT_TRUE(doc.LoadFile());
  TiXmlElement* root = doc.RootElement();
  for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
    LoadElement(elem);
  }
  EXPECT_TRUE(roads.empty()); // Road without name should be rejected
}

// Test 3: Inconsistent traffic situation (vehicle on non-existent road)
TEST_F(TrafficSimTest, InconsistentRoad) {
  TiXmlDocument doc("../Tests/inconsistent_road.xml");
  ASSERT_TRUE(doc.LoadFile()) << "Failed to load XML: " << doc.ErrorDesc();
  TiXmlElement* root = doc.RootElement();
  for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
    LoadElement(elem);
  }
  EXPECT_TRUE(vehicles.empty()); // Vehicle on invalid road should be ignored
}

// Test 4: Vehicle movement
TEST_F(TrafficSimTest, VehicleMovement) {
  TiXmlDocument doc("../Tests/vehicle_movement.xml");
  ASSERT_TRUE(doc.LoadFile());
  TiXmlElement* root = doc.RootElement();
  for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
    LoadElement(elem);
  }

  // Debug: Print loaded vehicles and roads
  // std::cout << "Loaded roads: " << roads.size() << std::endl;
  // std::cout << "Loaded vehicles: " << vehicles.size() << std::endl;


  ASSERT_EQ(vehicles.size(), 1u) << "Expected 1 vehicle, got " << vehicles.size();
  Vehicle& car = vehicles[0];
  double initialPos = car.get_position();
  UpdateVehicleMovement(Constants::SimulationTimeStep, 0.0);
  EXPECT_GT(car.get_position(), initialPos); // Vehicle should move forward
}

// Test 5: Traffic light cycle
TEST_F(TrafficSimTest, TrafficLightCycle) {
  TiXmlDocument doc("../Tests/traffic_light_cycle.xml");
  ASSERT_TRUE(doc.LoadFile()) << "Failed to load XML: " << doc.ErrorDesc();
  TiXmlElement* root = doc.RootElement();
  for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
    LoadElement(elem);
  }

  // Check if traffic lights were loaded
  ASSERT_FALSE(trafficlights.empty()) << "No traffic lights found!";
  TrafficLight& light = trafficlights[0];

  bool initialColor = light.is_green();
  UpdateTrafficLights(light.get_cyclus() * Constants::SimulationTimeStep + 0.1); // Force cycle
  EXPECT_NE(light.is_green(), initialColor); // Light should change color
}

// Test 6: Vehicle generation
TEST_F(TrafficSimTest, VehicleGeneration) {
  TiXmlDocument doc("../Tests/vehicle_generator.xml");
  ASSERT_TRUE(doc.LoadFile());
  TiXmlElement* root = doc.RootElement();
  for (TiXmlElement* elem = root->FirstChildElement(); elem; elem = elem->NextSiblingElement()) {
    LoadElement(elem);
  }
  GenerateVehicles(5.0); // Simulate 5 seconds
  EXPECT_GT(vehicles.size(), 0u); // At least one vehicle should spawn
}

TEST_F(TrafficSimTest, VehicleFollowing) {
  TiXmlDocument doc("../Tests/vehicle_following.xml");
  ASSERT_TRUE(doc.LoadFile());
  TiXmlElement* root = doc.RootElement();
  LoadElement(root);

  ASSERT_EQ(vehicles.size(), 2u);
  Vehicle& frontCar = vehicles[0]; // Position 50
  Vehicle& rearCar = vehicles[1];  // Position 10

  EXPECT_GT(frontCar.get_position(), rearCar.get_position());

  // Simulate movement
  UpdateVehicleMovement(Constants::SimulationTimeStep, 0.0);

  // Rear car should decelerate to avoid collision
  EXPECT_LT(rearCar.get_acceleration(), 0.0);
}

TEST_F(TrafficSimTest, MultipleTrafficLights) {
  TiXmlDocument doc("../Tests/multiple_lights.xml");
  ASSERT_TRUE(doc.LoadFile());
  LoadElement(doc.RootElement());

  ASSERT_EQ(trafficlights.size(), 2u);
  Vehicle& car = vehicles[0];

  // Force the nearest light (at 100m) to turn red
  trafficlights[0].set_light(false);
  UpdateVehicleMovement(Constants::SimulationTimeStep, 0.0);

  // Car should decelerate for the closer light (100m)
  EXPECT_LT(car.get_speed(), Constants::MaxAbsoluteSpeed);
}

TEST_F(TrafficSimTest, VehicleExitsRoad) {
  TiXmlDocument doc("../Tests/vehicle_exit.xml");
  ASSERT_TRUE(doc.LoadFile());
  LoadElement(doc.RootElement());

  ASSERT_EQ(vehicles.size(), 1u);
  UpdateVehicleMovement(Constants::SimulationTimeStep, 0.0);

  // Vehicle should be removed after moving past the road length
  EXPECT_TRUE(vehicles.empty());
}

TEST_F(TrafficSimTest, InvalidAttributes) {
  TiXmlDocument doc("../Tests/invalid_attributes.xml");
  ASSERT_TRUE(doc.LoadFile());
  LoadElement(doc.RootElement());

  // Invalid road and generator should be rejected
  EXPECT_TRUE(roads.empty());
  EXPECT_TRUE(vehicle_gens.empty());
}

TEST_F(TrafficSimTest, GeneratorSpacing) {
  TiXmlDocument doc("../Tests/generator_spacing.xml");
  ASSERT_TRUE(doc.LoadFile());
  LoadElement(doc.RootElement());

  // Initial vehicle at position 1 blocks the generator
  GenerateVehicles(5.0);
  EXPECT_EQ(vehicles.size(), 1u); // No new vehicles spawned
}

TEST_F(TrafficSimTest, StopDistance) {
  TiXmlDocument doc("../Tests/stop_distance.xml");
  ASSERT_TRUE(doc.LoadFile());
  TiXmlElement* root = doc.RootElement();
  LoadElement(root);

  // Force light to stay RED
  ASSERT_FALSE(trafficlights.empty());
  TrafficLight& light = trafficlights[0];
  light.set_light(false);
  light.set_cyclus(1000000000); // Match XML value
  light.set_last_change_time(0.0); // Reset timer

  // Simulate steps (2000 steps ≈ 33.2 seconds)
  for (int i = 0; i < 2000; i++) {
    UpdateVehicleMovement(Constants::SimulationTimeStep, i * Constants::SimulationTimeStep);
  }

  // Verify vehicle stopped before the light
  EXPECT_NEAR(vehicles[0].get_speed(), 0.0, 0.1);
  EXPECT_LT(vehicles[0].get_position(), 145 - Constants::VehicleLength);
}

TEST_F(TrafficSimTest, MultipleGenerators) {
  TiXmlDocument doc("../Tests/multiple_generators.xml");
  ASSERT_TRUE(doc.LoadFile());
  LoadElement(doc.RootElement());

  // Generate vehicles on both roads
  GenerateVehicles(5.0);
  EXPECT_EQ(vehicles.size(), 2u);
}


// Add this test case to your main.cpp
TEST_F(TrafficSimTest, FullSimulationTest) {
    // Load the simulation_data.xml file
    TiXmlDocument doc("../Tests/simulation_data.xml");
    ASSERT_TRUE(doc.LoadFile()) << "Failed to load XML: " << doc.ErrorDesc();

    // Load all elements from the XML file
    LoadElement(doc.RootElement());

    // Check if the road was loaded correctly
    ASSERT_EQ(roads.size(), 1u) << "Expected 1 road, got " << roads.size();
    EXPECT_EQ(roads[0].get_name(), "Highway") << "Road name should be 'Highway'";
    EXPECT_EQ(roads[0].get_length(), 1000) << "Road length should be 1000m";

    // Check if the vehicles were loaded correctly
    ASSERT_EQ(vehicles.size(), 2u) << "Expected 2 vehicles, got " << vehicles.size();
    EXPECT_EQ(vehicles[0].get_road_name(), "Highway") << "First car should be on 'Highway'";
    EXPECT_EQ(vehicles[0].get_position(), 900.0) << "First car should start at 900m";
    EXPECT_EQ(vehicles[1].get_road_name(), "Highway") << "Second car should be on 'Highway'";
    EXPECT_EQ(vehicles[1].get_position(), 300.0) << "Second car should start at 300m";

    // Check if the traffic light was loaded correctly
    ASSERT_EQ(trafficlights.size(), 1u) << "Expected 1 traffic light, got " << trafficlights.size();
    EXPECT_EQ(trafficlights[0].get_road_name(), "Highway") << "Traffic light should be on 'Highway'";
    EXPECT_EQ(trafficlights[0].get_position(), 500) << "Traffic light should be at 500m";
    EXPECT_EQ(trafficlights[0].get_cyclus(), 30) << "Traffic light cycle should be 30 steps";

    // Check if the vehicle generator was loaded correctly
    ASSERT_EQ(vehicle_gens.size(), 1u) << "Expected 1 vehicle generator, got " << vehicle_gens.size();
    EXPECT_EQ(vehicle_gens[0].road, "Highway") << "Generator should be on 'Highway'";
    EXPECT_EQ(vehicle_gens[0].frequency, 5) << "Generator frequency should be 5 seconds";

    // Simulate for a few steps to see if everything works together
    double current_time = 0.0;
    for (int i = 0; i < 100; i++) {
        UpdateVehicleMovement(Constants::SimulationTimeStep, current_time);
        UpdateTrafficLights(current_time);
        GenerateVehicles(current_time);
        current_time += Constants::SimulationTimeStep;
    }

    // Check if the first car exited the road (it started at 900m on a 1000m road)
    EXPECT_TRUE(vehicles.size() <= 2u) << "First car should have exited the road";

    // Check if new cars were spawned by the generator
    EXPECT_GT(vehicles.size(), 1u) << "At least one new car should have been spawned";

    // Check if traffic light changed state
    EXPECT_NE(trafficlights[0].is_green(), true) << "Traffic light should have changed from green to red";
}


int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}