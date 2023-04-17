#include <array>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "aircraft.h"
#include "aircraft_factory.h"
#include "charging_station.h"

// TODO: make this into a class

void update_aircraft(std::shared_ptr<Aircraft> aircraft, ChargingStation& charging_station, double dt_hours) {
    // Complete flying action or charging action
    if (aircraft->get_state() == AircraftState::Flying) {
        aircraft->fly(dt_hours);
    } 
    else if (aircraft->get_state() == AircraftState::Charging) {
        aircraft->charge(dt_hours);
    }

    // If aircraft is flying and battery is dead, transition Flying -> Charging or Waiting
    if (aircraft->get_state() == AircraftState::Flying && aircraft->is_battery_dead()) {
        // If a charger is available, transition to charging
        if (charging_station.is_charger_available()) {
            charging_station.add_to_charger();
            aircraft->set_state(AircraftState::Charging);
        } 
        // If no charger is available, transition to waiting and enter queue
        else {
            charging_station.add_to_queue(aircraft);
            aircraft->set_state(AircraftState::Waiting);
        }
    }

    // If aircraft is charging and battery is full, transition Charging -> Flying
    else if (aircraft->get_state() == AircraftState::Charging && aircraft->is_battery_full()) {
        charging_station.remove_from_charger();
        aircraft->set_state(AircraftState::Flying);
    }

    // If aircraft is waiting, a charger is available, and it's next in line in the queue, transition Waiting -> Flying
    else if (aircraft->get_state() == AircraftState::Waiting && charging_station.is_charger_available()) {
        if (charging_station.get_next_in_queue() == aircraft) {
            charging_station.remove_from_queue();
            charging_station.add_to_charger();
            aircraft->set_state(AircraftState::Charging);
        }
    }
}

void run_simulation(std::array<std::shared_ptr<Aircraft>, 20>& vehicles) {
    // Run simulation for 3 hours with timestep of 1 second (1/3600 hours)
    double simulation_length_hours = 3.0;
    double dt_hours = 1.0 / 3600.0;

    int number_chargers = 3;
    ChargingStation charging_station(number_chargers);

    // Update each vehicle at every timestep
    for (double i = 0; i < simulation_length_hours; i += dt_hours) {
        for (auto aircraft : vehicles) {
            update_aircraft(aircraft, charging_station, dt_hours);
        }
    }
}

std::string format_time(double hours) {
    int whole_hours = static_cast<int>(hours);
    int minutes = static_cast<int>((hours - whole_hours) * 60);

    std::ostringstream oss;
    oss << whole_hours << ':' << std::setw(2) << std::setfill('0') << minutes;
    return oss.str();
}

void print_row(std::string aircraft_name, Metrics metrics) {
    std::cout << std::setw(15) << aircraft_name
              << std::fixed << std::setprecision(1)
              << std::setw(18) << format_time(metrics.avg_flight_time)
              << std::setw(20) << metrics.avg_flight_distance
              << std::setw(18) << format_time(metrics.avg_charging_time)
              << std::setw(10) << static_cast<int>(metrics.total_faults)
              << std::setw(18) << metrics.total_passenger_miles << std::endl;
}


void print_results(std::array<std::shared_ptr<Aircraft>, 20>& vehicles) {
    std::array<std::vector<Metrics>, 5> metrics_by_aircraft_type;
    std::array<std::string, 5> enum_strings = {"Alpha", "Bravo", "Charlie", "Delta", "Echo"};

    // Compute metrics for each vehicle and store by vehicle type
    for (const auto vehicle : vehicles) {
        Metrics metrics = vehicle->compute_metrics();
        metrics_by_aircraft_type[vehicle->get_type()].push_back(metrics);
    }

    // Print table header
    std::cout << std::setw(15) << "Name"
              << std::setw(18) << "Avg Flight Time"
              << std::setw(20) << "Avg Flight Distance"
              << std::setw(18) << "Avg Charging Time"
              << std::setw(10) << "Faults"
              << std::setw(18) << "Passenger Miles" << std::endl;

    // Iterate over metrics for each type of aircraft
    for (int i = 0; i < metrics_by_aircraft_type.size(); i++) {
        for (int j = 0; j < metrics_by_aircraft_type[i].size(); j++) {
            std::string aircraft_name = enum_strings[i] + " " + std::to_string(j);
            print_row(aircraft_name, metrics_by_aircraft_type[i][j]);
        }
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
