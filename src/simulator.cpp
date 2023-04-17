#include <array>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "aircraft.h"
#include "aircraft_factory.h"
#include "charging_station.h"

struct SimParams {
    int number_vehicles;
    int number_chargers;
    double dt;              // simulation timestep (hours)
    double sim_length;      // simulation total length (hours)
};

void update_aircraft(std::shared_ptr<Aircraft> aircraft, ChargingStation& charging_station, double dt) {
    // Complete flying action or charging action
    if (aircraft->get_state() == AircraftState::Flying) {
        aircraft->fly(dt);
    } 
    else if (aircraft->get_state() == AircraftState::Charging) {
        aircraft->charge(dt);
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

std::vector<std::vector<Metrics>> run_simulation(SimParams& sim_params) {
    ChargingStation charging_station(sim_params.number_chargers);
    std::vector<std::shared_ptr<Aircraft>> vehicles;
    std::vector<std::vector<Metrics>> metrics_by_vehicle_type;

    // Randomly select aircraft types and create vehicles
    for (int i = 0; i < sim_params.number_vehicles; i++) {
        AircraftType type = static_cast<AircraftType>(rand() % AircraftType::Count);
        vehicles[i] = AircraftFactory::create_aircraft(type);
    }

    // Update each vehicle at every timestep
    for (double i = 0; i < sim_params.sim_length; i += sim_params.dt) {
        for (const auto& aircraft : vehicles) {
            update_aircraft(aircraft, charging_station, sim_params.dt);
        }
    }

    // Compute metrics for each vehicle and store by vehicle type
    for (const auto& vehicle : vehicles) {
        Metrics metrics = vehicle->compute_metrics();
        metrics_by_vehicle_type[vehicle->get_type()].push_back(metrics);
    }

    return metrics_by_vehicle_type;
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

void print_results(std::vector<std::vector<Metrics>>& metrics) {
    std::array<std::string, 5> enum_strings = {"Alpha", "Bravo", "Charlie", "Delta", "Echo"};

    // Print table header
    std::cout << std::setw(15) << "Name"
              << std::setw(18) << "Avg Flight Time"
              << std::setw(20) << "Avg Flight Distance"
              << std::setw(18) << "Avg Charging Time"
              << std::setw(10) << "Faults"
              << std::setw(18) << "Passenger Miles" << std::endl;

    // Iterate over metrics for each type of aircraft
    for (int i = 0; i < metrics.size(); i++) {
        for (int j = 0; j < metrics[i].size(); j++) {
            std::string aircraft_name = enum_strings[i] + " " + std::to_string(j);
            print_row(aircraft_name, metrics[i][j]);
        }
    }
}

int main() {
    // Set up simulation with 20 vehicles, 3 chargers, 1 second timestep, and 3 hours total length
    SimParams sim_params = {
        .number_vehicles = 20,
        .number_chargers = 3,
        .dt = 1.0 / 3600.0,
        .sim_length = 3.0
    };

    std::vector<std::vector<Metrics>> metrics = run_simulation(sim_params);
    print_results(metrics);

    return 0;
}
