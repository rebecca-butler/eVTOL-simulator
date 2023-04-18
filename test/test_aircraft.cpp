#include <gtest/gtest.h>
#include <memory>

#include "aircraft.h"
#include "charging_station.h"

// Test class 
class TestAircraft : public Aircraft {
public:
    TestAircraft() {
        cruise_speed = 100;
        battery_capacity = 200;
        charging_time = 0.5;
        energy_use = 2.0;
        faults_per_hour_prob = 0.5;
        number_passengers = 2;
        current_battery = battery_capacity;
    }

    double get_total_distance_travelled() { 
        return total_distance_travelled;
    }

    double get_current_battery() { 
        return current_battery;
    }

    void set_current_battery(double current_battery_) { 
        current_battery = current_battery_;
    }

    double get_battery_capacity() { 
        return battery_capacity;
    }
};

TEST(AircraftTest, Fly) {
    // Fly the aircraft for 0.1 hours
    TestAircraft aircraft;
    aircraft.fly(0.1);

    // At 100 mph for 0.1 hours, total distance flown should be 10 miles
    EXPECT_FLOAT_EQ(aircraft.get_total_distance_travelled(), 10);

    // With an energy use of 2 kWh/mile over 10 miles, battery should have drained by 20 kW
    EXPECT_FLOAT_EQ(aircraft.get_current_battery(), aircraft.get_battery_capacity() - 20);
}

TEST(AircraftTest, Charge) {
    // Set the aircraft battery to 0
    TestAircraft aircraft;
    aircraft.set_current_battery(0.0);
    EXPECT_TRUE(aircraft.is_battery_dead());

    // Charge the aircraft for 0.25 hours
    aircraft.charge(0.25);

    // After charging for 0.25 hours with a total time to charge of 0.5 hours, the battery should be half full
    EXPECT_FLOAT_EQ(aircraft.get_current_battery(), aircraft.get_battery_capacity() / 2.0);

    // Continue charging for 5 hours
    aircraft.charge(5.0);

    // Verify that the battery is full and didn't go higher than the maximum
    EXPECT_TRUE(aircraft.is_battery_full());
    EXPECT_FLOAT_EQ(aircraft.get_current_battery(), aircraft.get_battery_capacity());
}

TEST(AircraftTest, StateTransition) {
    // Create a test aircraft and charging station with 1 charger
    std::shared_ptr<TestAircraft> aircraft = std::make_shared<TestAircraft>();
    ChargingStation charging_station(1);

    // After flying for 1.2 hours, aircraft should transition to OutOfBattery state
    EXPECT_TRUE(aircraft->get_state() == AircraftState::Flying);
    aircraft->update_state(1.2);
    EXPECT_TRUE(aircraft->get_state() == AircraftState::OutOfBattery);

    // After being added to charging queue, aircraft should transition to Charging state
    charging_station.add_to_queue(aircraft);
    charging_station.update();
    EXPECT_TRUE(aircraft->get_state() == AircraftState::Charging);

    // After charging for 0.6 hours, aircraft should transition to Flying state
    aircraft->update_state(0.6);
    charging_station.update();
    EXPECT_TRUE(aircraft->get_state() == AircraftState::Flying);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
