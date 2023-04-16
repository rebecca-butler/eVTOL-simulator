#include <array>
#include <iostream>
#include <memory>

#include "aircraft.h"

struct Metrics {
    double avg_flight_time;
    double avg_flight_distance;
    double avg_charging_time;
    double total_passenger_miles;
    double total_faults;
};

void update_aircraft(std::shared_ptr<Aircraft> aircraft, double dt_seconds, int number_aircraft_charging) {
    // TODO: implement a getter for the state
    if (aircraft->state == AircraftState::Flying) {
        aircraft->fly(dt_seconds);
    } 

    else if (aircraft->state == AircraftState::Charging) {
        aircraft->charge(dt_seconds);
    }

    // After completing the update, transition to new state if applicable
    // TODO: don't return number_aircraft_charging, it's confusing. Find different way to update
    number_aircraft_charging = aircraft->transition_state(number_aircraft_charging);
}

void run_simulation(std::array<std::shared_ptr<Aircraft>, 20>& vehicles) {
    double dt_seconds = 1;
    double simulation_length_seconds = 3 * 3600;
    int number_aircraft_charging = 0;

    for (double i = 0; i < simulation_length_seconds; i += dt_seconds) {
        for (auto aircraft : vehicles) {
            update_aircraft(aircraft, dt_seconds, number_aircraft_charging);
        }
    }
}

void print_results(std::array<std::shared_ptr<Aircraft>, 20>& vehicles) {
    std::array<Metrics, 4> metrics;

    for (const auto vehicle : vehicles) {
        // Compute metrics for vehicle

        // Update metrics array for vehicles of this type
    }

    // Print as a formatted table
}

int main() {
    int number_vehicles = 20;
    std::array<std::shared_ptr<Aircraft>, 20> vehicles;

    for (int i = 0; i < number_vehicles; i++) {
        // Randomly select an aircraft type and create object
        AircraftType type = static_cast<AircraftType>(rand() % AircraftType::Count);
        vehicles[i] = AircraftFactory::create_aircraft(type);
    }

    run_simulation(vehicles);
    print_results(vehicles);

    return 0;
}
