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

std::vector<std::vector<Metrics>> run_simulation(SimParams& sim_params) {
    ChargingStation charging_station(sim_params.number_chargers);
    std::vector<std::shared_ptr<Aircraft>> vehicles;
    std::vector<std::vector<Metrics>> metrics(AircraftType::Count);

    // Randomly select aircraft types and create vehicles
    for (int i = 0; i < sim_params.number_vehicles; i++) {
        AircraftType type = static_cast<AircraftType>(rand() % AircraftType::Count);
        vehicles.push_back(AircraftFactory::create_aircraft(type));
    }

    // Update each vehicle at every timestep
    for (double i = 0; i < sim_params.sim_length; i += sim_params.dt) {
        for (auto& aircraft : vehicles) {
            aircraft->update_state(sim_params.dt);
            if (aircraft->get_state() == AircraftState::OutOfBattery) {
                charging_station.add_to_queue(aircraft);
                aircraft->set_state(AircraftState::InQueue);
            }
        }
        charging_station.update();
    }

    // Compute metrics for each vehicle and store by vehicle type
    for (const auto& vehicle : vehicles) {
        Metrics metric = vehicle->compute_metrics();
        metrics[vehicle->get_type()].push_back(metric);
    }

    return metrics;
}

std::string format_time(double hours) {
    int whole_hours = static_cast<int>(hours);
    int minutes = static_cast<int>((hours - whole_hours) * 60);

    std::ostringstream oss;
    oss << whole_hours << ':' << std::setw(2) << std::setfill('0') << minutes;
    return oss.str();
}

void print_header() {
    std::cout << std::setw(15) << "Name"
              << std::setw(15) << "Num Flights"
              << std::setw(18) << "Avg Flight Time"
              << std::setw(22) << "Avg Flight Distance"
              << std::setw(15) << "Num Charges"
              << std::setw(18) << "Avg Charge Time"
              << std::setw(10) << "Faults"
              << std::setw(18) << "Passenger Miles" << std::endl;
}

void print_row(std::string aircraft_name, Metrics metrics) {
    std::cout << std::setw(15) << aircraft_name
              << std::fixed << std::setprecision(1)
              << std::setw(15) << metrics.number_flights
              << std::setw(18) << format_time(metrics.avg_flight_time)
              << std::setw(22) << metrics.avg_flight_distance
              << std::setw(15) << metrics.number_charges
              << std::setw(18) << format_time(metrics.avg_charging_time)
              << std::setw(10) << static_cast<int>(metrics.total_faults)
              << std::setw(18) << metrics.total_passenger_miles << std::endl;
}

std::vector<Metrics> get_metrics_per_type(std::vector<std::vector<Metrics>>& metrics) {
    std::vector<Metrics> metrics_per_type(AircraftType::Count);

    // Iterate over metrics for each aircraft
    for (int i = 0; i < metrics.size(); i++) {
        for (int j = 0; j < metrics[i].size(); j++) {

            // Update running total metrics for this aircraft type
            metrics_per_type[i].avg_flight_time += metrics[i][j].avg_flight_time;
            metrics_per_type[i].avg_charging_time += metrics[i][j].avg_charging_time;
            metrics_per_type[i].avg_flight_distance += metrics[i][j].avg_flight_distance;
            metrics_per_type[i].total_passenger_miles += metrics[i][j].total_passenger_miles;
            metrics_per_type[i].total_faults += metrics[i][j].total_faults;
            metrics_per_type[i].number_flights += metrics[i][j].number_flights;
            metrics_per_type[i].number_charges += metrics[i][j].number_charges;
        }
    }

    // Compute averages
    for (int i = 0; i < metrics_per_type.size(); i++) {
        metrics_per_type[i].avg_flight_time /= metrics[i].size();
        metrics_per_type[i].avg_charging_time /= metrics[i].size();
        metrics_per_type[i].avg_flight_distance /= metrics[i].size();
    }

    return metrics_per_type;
}

void print_results(std::vector<std::vector<Metrics>>& metrics) {
    std::array<std::string, AircraftType::Count> enum_strings = {"Alpha", "Bravo", "Charlie", "Delta", "Echo"};

    // Create table with each aircraft's metrics
    print_header();

    for (int i = 0; i < metrics.size(); i++) {
        for (int j = 0; j < metrics[i].size(); j++) {
            std::string aircraft_name = enum_strings[i] + " " + std::to_string(j);
            print_row(aircraft_name, metrics[i][j]);
        }
    }

    // Create table with metrics for each type
    std::cout << "\n";
    print_header();
    std::vector<Metrics> metrics_per_type = get_metrics_per_type(metrics);

    for (int i = 0; i < metrics_per_type.size(); i++) {
        std::string aircraft_name = enum_strings[i] + " Total";
        print_row(aircraft_name, metrics_per_type[i]);
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
