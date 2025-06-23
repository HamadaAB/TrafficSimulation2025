#include "TrafficSimulation.h"
#include "tinyxml/tinyxml.h"
#include <gtest/gtest.h>



// Test Fixture
class TrafficSimTest : public ::testing::Test {
protected:
        // void SetUp() override {}
};

//===== Test Cases =====

// Test 1: Invalid XML element
TEST_F(TrafficSimTest, InvalidElement) {
    std::string test = "../Tests/invalid_element.xml";
    TrafficSimulation sim(test);

    // !!!
    // EXPECT_TRUE(roads.empty() && vehicles.empty() && vehicle_gens.empty() && trafficlights.empty()
    //                        && bushalten.empty() && kruispunten.empty()); // Invalid element should not load
}

// Test 2: Missing required attribute
TEST_F(TrafficSimTest, MissingAttribute) {
    TrafficSimulation sim("../Tests/missing_attribute.xml");

    // EXPECT_TRUE(result == 2); // Road without name should be rejected
}

// Test 3: Inconsistent traffic situation (vehicle on non-existent road)
TEST_F(TrafficSimTest, InconsistentRoad) {
    TrafficSimulation sim("../Tests/inconsistent_road.xml");
    // EXPECT_TRUE(result == 4); // Vehicle on invalid road should be ignored
}

// Test 4: Vehicle movement
TEST_F(TrafficSimTest, VehicleMovement) {
    TrafficSimulation sim("../Tests/vehicle_movement.xml");


    // Debug: Print loaded vehicles and roads
    // std::cout << "Loaded roads: " << roads.size() << std::endl;
    // std::cout << "Loaded vehicles: " << vehicles.size() << std::endl;

    std::list<Vehicle> vehicles = sim.get_vehicles();
    ASSERT_EQ(vehicles.size(), 1u) << "Expected 1 vehicle, got " << vehicles.size();
    Vehicle car = vehicles.front();
    double initialPos = car.get_position();

    sim.UpdateVehicleMovement(Constants::SimulationTimeStep, 0.0);

    vehicles = sim.get_vehicles();
    car = vehicles.front();

    EXPECT_GT(car.get_position(), initialPos); // Vehicle should move forward
}

// Test 5: Traffic light cycle
TEST_F(TrafficSimTest, TrafficLightCycle) {
    TrafficSimulation sim("../Tests/traffic_light_cycle.xml");

    std::list<TrafficLight> trafficlights = sim.get_lights();
    // Check if traffic lights were loaded
    ASSERT_FALSE(trafficlights.empty()) << "No traffic lights found!";
    TrafficLight& light = trafficlights.front();

    bool initialColor = light.is_green();
    sim.UpdateTrafficLights(light.get_cyclus() * Constants::SimulationTimeStep + 0.1); // Force cycle

    light = sim.get_lights().front();

    EXPECT_NE(light.is_green(), initialColor); // Light should change color

}

// Test 6: Vehicle generation
TEST_F(TrafficSimTest, VehicleGeneration) {
    TrafficSimulation sim("../Tests/vehicle_generator.xml");

    sim.GenerateVehicles(5.0); // Simulate 5 seconds
    EXPECT_GT(sim.get_vehicles().size(), 0u); // At least one vehicle should spawn
}

TEST_F(TrafficSimTest, VehicleFollowing) {
    TrafficSimulation sim("../Tests/vehicle_following.xml");

    std::list<Vehicle> vehicles = sim.get_vehicles();
    ASSERT_EQ(vehicles.size(), 2u);
    Vehicle& frontCar = vehicles.front(); // Position 50
    Vehicle& rearCar = vehicles.back();    // Position 10

    EXPECT_GT(frontCar.get_position(), rearCar.get_position());

    // Simulate movement
    sim.UpdateVehicleMovement(Constants::SimulationTimeStep, 0.0);

    // Rear car should decelerate to avoid collision
    EXPECT_LT(rearCar.get_acceleration(), 0.0);

}

TEST_F(TrafficSimTest, MultipleTrafficLights) {
    TrafficSimulation sim("../Tests/multiple_lights.xml");

    /*
    ASSERT_EQ(trafficlights.size(), 2u);
    Vehicle& car = vehicles[0];

    // Force the nearest light (at 100m) to turn red
    trafficlights[0].set_light(false);
    UpdateVehicleMovement(Constants::SimulationTimeStep, 0.0);

    // Car should decelerate for the closer light (100m)
    EXPECT_LT(car.get_speed(), Constants::MaxAbsoluteSpeed);
     */
}

TEST_F(TrafficSimTest, VehicleExitsRoad) {
    TrafficSimulation sim("../Tests/vehicle_exit.xml");

    std::list<Vehicle> vehicles = sim.get_vehicles();
    ASSERT_EQ(vehicles.size(), 1u);
    sim.UpdateVehicleMovement(Constants::SimulationTimeStep, 0.0);

    vehicles = sim.get_vehicles();
    // Vehicle should be removed after moving past the road length
    EXPECT_TRUE(vehicles.empty());
}

TEST_F(TrafficSimTest, InvalidAttributes) {
    TrafficSimulation sim("../Tests/invalid_attributes.xml");


    // Invalid road and generator should be rejected
    // EXPECT_TRUE(roads.empty());
    // EXPECT_TRUE(vehicle_gens.empty());
}

TEST_F(TrafficSimTest, GeneratorSpacing) {
    TrafficSimulation sim("../Tests/generator_spacing.xml");

    // Initial vehicle at position 1 blocks the generator
    sim.GenerateVehicles(5.0);
    EXPECT_EQ(sim.get_vehicles().size(), 1u); // No new vehicles spawned
}

TEST_F(TrafficSimTest, StopDistance) {
    TrafficSimulation sim("../Tests/stop_distance.xml");

    std::list<TrafficLight> trafficlights = sim.get_lights();
    // Force light to stay RED
    ASSERT_FALSE(trafficlights.empty());
    TrafficLight& light = trafficlights.front();
    light.set_light(false);
    light.set_cyclus(1000000000); // Match XML value
    light.set_last_change_time(0.0); // Reset timer

    // Simulate steps (2000 steps ≈ 33.2 seconds)
    for (int i = 0; i < 2000; i++) {
        sim.UpdateVehicleMovement(Constants::SimulationTimeStep, i * Constants::SimulationTimeStep);
    }

    std::list<Vehicle> vehicles = sim.get_vehicles();
    // Verify vehicle stopped before the light
    EXPECT_NEAR(vehicles.front().get_speed(), 0.0, 0.1);
    EXPECT_LT(vehicles.front().get_position(), 145 - Constants::VehicleLength);

}

TEST_F(TrafficSimTest, MultipleGenerators) {
    TrafficSimulation sim("../Tests/multiple_generators.xml");


    // Generate vehicles on both roads
    sim.GenerateVehicles(5.0);
    // EXPECT_EQ(vehicles.size(), 2u);
}

TEST_F(TrafficSimTest, missingDataBusstop) {
    TrafficSimulation sim("../Tests/missingDataBusstop.xml");

    // EXPECT_TRUE(result == 2);
}

TEST_F(TrafficSimTest, missingDataCrossroad) {
    TrafficSimulation sim("../Tests/missingDataCrossroad.xml");

    // EXPECT_TRUE(result == 2);
}

TEST_F(TrafficSimTest, UpdateCycle) {
    TrafficSimulation sim("../Tests/UpdateCycle.xml");

    sim.update_cycle();
    EXPECT_TRUE(sim.get_vehicles().empty());
}

/*
TEST_F(TrafficSimTest, FullSimulationTest) {
    // Load the simulation_data.xml file
    TrafficSimulation sim("../Tests/simulation_data.xml");

    std::vector<Road> roads = sim.get_roads();
    // Check if the road was loaded correctly
    ASSERT_EQ(roads.size(), 1u) << "Expected 1 road, got " << roads.size();
    EXPECT_EQ(roads[0].get_name(), "Highway") << "Road name should be 'Highway'";
    EXPECT_EQ(roads[0].get_length(), 1000) << "Road length should be 1000m";

    std::vector<Vehicle> vehicles = sim.get_vehicles();
    // Check if the vehicles were loaded correctly
    ASSERT_EQ(vehicles.size(), 2u) << "Expected 2 vehicles, got " << vehicles.size();
    EXPECT_EQ(vehicles[0].get_road_name(), "Highway") << "First car should be on 'Highway'";
    EXPECT_EQ(vehicles[0].get_position(), 900.0) << "First car should start at 900m";
    EXPECT_EQ(vehicles[1].get_road_name(), "Highway") << "Second car should be on 'Highway'";
    EXPECT_EQ(vehicles[1].get_position(), 300.0) << "Second car should start at 300m";

    std::vector<TrafficLight> trafficlights = sim.get_lights();
    // Check if the traffic light was loaded correctly
    ASSERT_EQ(trafficlights.size(), 1u) << "Expected 1 traffic light, got " << trafficlights.size();
    EXPECT_EQ(trafficlights[0].get_road_name(), "Highway") << "Traffic light should be on 'Highway'";
    EXPECT_EQ(trafficlights[0].get_position(), 500) << "Traffic light should be at 500m";
    EXPECT_EQ(trafficlights[0].get_cyclus(), 30) << "Traffic light cycle should be 30 steps";

    std::vector<VehicleGenerator> vehicle_gens = sim.get_generators();
    // Check if the vehicle generator was loaded correctly
    ASSERT_EQ(vehicle_gens.size(), 1u) << "Expected 1 vehicle generator, got " << vehicle_gens.size();
    EXPECT_EQ(vehicle_gens[0].get_road()->get_name(), "Highway") << "Generator should be on 'Highway'";
    EXPECT_EQ(vehicle_gens[0].get_frequency(), 5) << "Generator frequency should be 5 seconds";

    // Simulate for a few steps to see if everything works together
    double current_time = 0.0;
    for (int i = 0; i < 100; i++) {
        sim.UpdateVehicleMovement(Constants::SimulationTimeStep, current_time);
        sim.UpdateTrafficLights(current_time);
        sim.GenerateVehicles(current_time);
        current_time += Constants::SimulationTimeStep;
    }

    // Check if the first car exited the road (it started at 900m on a 1000m road)
    EXPECT_TRUE(vehicles.size() <= 2u) << "First car should have exited the road";

    // Check if new cars were spawned by the generator
    EXPECT_GT(vehicles.size(), 1u) << "At least one new car should have been spawned";

    // Check if traffic light changed state
    EXPECT_TRUE(trafficlights[0].is_green()) << "Traffic light should have changed from green to red";

}
*/

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}