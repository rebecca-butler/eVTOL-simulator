#include <random>

#include "aircraft.h"

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

AircraftState Aircraft::get_state() {
    return state;
}

void Aircraft::set_state(AircraftState state_){
    state = state_;
}

bool Aircraft::is_battery_full() {
    return current_battery == battery_capacity;
}

bool Aircraft::is_battery_dead() {
    return current_battery == 0;
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
    // If battery is dead, this is the start of a new charging cycle. Increment number of times charged
    if (is_battery_dead()) {
        number_charges++;
    }

    // Increase battery level. Units: (kWh) = (kWh) / (hr) * (hr) 
    current_battery += battery_capacity / charging_time * dt;
    current_battery = std::min(current_battery, battery_capacity);

    // Update metrics
    total_charging_time += dt;
}

void Aircraft::update_state(double dt) {
    // Complete flying action or charging action
    if (state == AircraftState::Flying) {
        fly(dt);
    } 
    else if (state == AircraftState::Charging) {
        charge(dt);
    }

    // If aircraft is flying and battery is dead, transition Flying -> OutOfBattery
    if (state == AircraftState::Flying && is_battery_dead()) {
        state = AircraftState::OutOfBattery;
    }

    // If aircraft is charging and battery is full, transition Charging -> Flying
    else if (state == AircraftState::Charging && is_battery_full()) {
        number_flights++;
        state = AircraftState::Flying;
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
    metrics.number_flights = number_flights;
    metrics.number_charges = number_charges;
    metrics.avg_flight_time = total_flight_time / number_flights;
    metrics.avg_charging_time = (number_charges == 0) ? 0 : (total_charging_time / number_charges);
    metrics.avg_flight_distance = total_distance_travelled / number_flights;
    metrics.total_passenger_miles = total_distance_travelled * number_passengers;
    metrics.total_faults = compute_faults();

    return metrics;
}
