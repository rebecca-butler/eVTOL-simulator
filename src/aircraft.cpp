#include <random>

#include "aircraft.h"
#include "charging_station.h"

Aircraft::Aircraft() {
    state = AircraftState::Flying;
    number_flights = 1;
    number_charges = 0;
    number_faults = 0;
    total_flight_time = 0;
    total_charging_time = 0;
    total_distance_travelled = 0;
}

AircraftType Aircraft::get_type() {
    return type;
}

bool Aircraft::is_battery_full() {
    return current_battery >= battery_capacity;
}

bool Aircraft::is_battery_dead() {
    return current_battery <= 0;
}

void Aircraft::fly(double dt) {
    // Compute distance travelled over the timestep. Units: (miles) = (miles/hr) * (hr)
    double distance = cruise_speed * dt;

    // Update metrics
    total_flight_time += dt;
    total_distance_travelled += distance;

    // Decrease battery level. Units: (kWh) = (kWh/mile) * (miles)
    current_battery -= energy_use * distance;
    current_battery = std::max(current_battery, 0.0);
}

void Aircraft::charge(double dt) {
    // Increase battery level. Units: (kWh/sec) = (kWh) / (hours) * (1 hr / 3600 sec) 
    current_battery += battery_capacity / charging_time / 3600.0;
    current_battery = std::min(current_battery, battery_capacity);

    // Update metrics
    total_charging_time += dt;
}

void Aircraft::update_state(ChargingStation& charging_station, double dt) {
    // Complete flying action or charging action
    if (state == AircraftState::Flying) {
        fly(dt);
    } 
    else if (state == AircraftState::Charging) {
        charge(dt);
    }

    // If aircraft is flying and battery is dead, transition Flying -> Charging or Waiting
    if (state == AircraftState::Flying && is_battery_dead()) {
        // If a charger is available, transition to charging
        if (charging_station.is_charger_available()) {
            charging_station.add_to_charger();
            number_charges++;
            state = AircraftState::Charging;
        } 
        // If no charger is available, transition to waiting and enter queue
        else {
            charging_station.add_to_queue(shared_from_this());
            state = AircraftState::Waiting;
        }
    }

    // If aircraft is charging and battery is full, transition Charging -> Flying
    else if (state == AircraftState::Charging && is_battery_full()) {
        charging_station.remove_from_charger();
        number_flights++;
        state = AircraftState::Flying;
    }

    // If aircraft is waiting, a charger is available, and it's next in line in the queue, transition Waiting -> Flying
    else if (state == AircraftState::Waiting && charging_station.is_charger_available()) {
        if (charging_station.get_next_in_queue() == shared_from_this()) {
            charging_station.remove_from_queue();
            charging_station.add_to_charger();
            number_charges++;
            state = AircraftState::Charging;
        }
    }
}

int Aircraft::compute_faults() {
    // Compute number of faults using Poisson distribution and probability of faults
    double expected_faults = faults_per_hour_prob * total_flight_time;
    std::poisson_distribution<int> distribution(expected_faults);
    std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}

Metrics Aircraft::compute_metrics() {
    Metrics metrics;
    metrics.avg_flight_time = total_flight_time / number_flights;
    metrics.avg_charging_time = (number_charges == 0) ? 0 : (total_charging_time / number_charges);
    metrics.avg_flight_distance = total_distance_travelled / number_flights;
    metrics.total_passenger_miles = total_distance_travelled * number_passengers;
    metrics.total_faults = compute_faults();

    return metrics;
}
