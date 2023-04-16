#include <array>
#include <iostream>
#include <memory>

#include "aircraft.h"

void update_aircraft(std::shared_ptr<Aircraft> aircraft, double dt_seconds) {
    // TODO: implement a getter for the state
    if (aircraft->state == AircraftState::Flying) {
        aircraft->fly(dt_seconds);
    } 

    else if (aircraft->state == AircraftState::Charging) {
        aircraft->charge();
    }

    else if (aircraft->state == AircraftState::Waiting) {
        aircraft->wait();
    }

    // After completing the update, transition to new state if applicable
    aircraft->transition_state();
}

void run_simulation(std::array<std::shared_ptr<Aircraft>, 20>& vehicles) {
    double dt_seconds = 1;
    double simulation_length_seconds = 3 * 3600;

    for (double i = 0; i < simulation_length_seconds; i += dt_seconds) {
        for (auto aircraft : vehicles) {
            update_aircraft(aircraft, dt_seconds);
        }
    }
}

void print_results(std::array<std::shared_ptr<Aircraft>, 20>& vehicles) {
    for (const auto vehicle : vehicles) {
        std::cout << vehicle->type << std::endl;
    }
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
