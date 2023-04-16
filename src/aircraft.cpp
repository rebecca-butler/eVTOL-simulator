#include <memory>
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

void Aircraft::fly(double dt_hours) {
    // Compute distance travelled over the timestep
    // TODO: add units to var names
    // Units: (miles) = (miles/hr) * (hr)
    double distance_miles = cruise_speed * dt_hours;

    // Update metrics
    total_flight_time += dt_hours;
    total_distance_travelled += distance_miles;

    // Decrease battery level
    // Units: (kWh) = (kWh/mile) * (miles)
    current_battery -= energy_use * distance_miles;
}

void Aircraft::charge(double dt_hours) {
    // Increase battery level
    // Units: (kWh/sec) = (kWh) / (hours) * (1 hr / 3600 sec) 
    current_battery += battery_capacity / charging_time * 1/3600;

    // Update metrics
    total_charging_time += dt_hours;
}

int Aircraft::transition_state(int& number_aircraft_charging) {
    // If currently flying, transition if battery is dead
    if (state == AircraftState::Flying && current_battery <= 0) {
        // If a charger is available, transition to charging state. Else transition to waiting state
        if (number_aircraft_charging < 3) {
            number_aircraft_charging++;
            number_charges++;
            state = AircraftState::Charging;
        } else {
            state = AircraftState::Waiting;
        }
    }

    // If currently charging, transition if battery is full
    else if (state == AircraftState::Charging && current_battery >= battery_capacity) {
        // Transition to flying state
        number_aircraft_charging--;
        number_flights++;
        state = AircraftState::Flying;
    }

    // If currently waiting, transition if a charger is available
    // TODO: plane should only be able to charge if it's next in line in the queue
    else if (state == AircraftState::Waiting) {
        // Transition to charging state
        if (number_aircraft_charging < 3) {
            number_aircraft_charging++;
            number_charges++;
            state = AircraftState::Charging;
        }
    }

    return number_aircraft_charging;
}

int Aircraft::compute_faults() {
    double expected_faults = faults_per_hour_prob * total_flight_time;
    std::poisson_distribution<int> distribution(expected_faults);
    std::mt19937 generator(std::random_device{}());
    return distribution(generator);
}


Metrics Aircraft::compute_metrics() {
    Metrics metrics;
    metrics.avg_flight_time = total_flight_time / number_flights;
    metrics.avg_charging_time = total_charging_time / number_charges;
    metrics.avg_flight_distance = total_distance_travelled / number_flights;
    metrics.total_passenger_miles = total_distance_travelled * number_passengers;
    metrics.total_faults = compute_faults();

    return metrics;
}

AlphaAircraft::AlphaAircraft() {
    cruise_speed = 120;
    battery_capacity = 320;
    charging_time = 0.6;
    energy_use = 1.6;
    faults_per_hour_prob = 0.25;
    number_passengers = 4;
    current_battery = battery_capacity;
    type = AircraftType::Alpha;
}

BravoAircraft::BravoAircraft() {
    cruise_speed = 100;
    battery_capacity = 100;
    charging_time = 0.2;
    energy_use = 1.5;
    faults_per_hour_prob = 0.1;
    number_passengers = 5;
    current_battery = battery_capacity;
    type = AircraftType::Bravo;
}

CharlieAircraft::CharlieAircraft() {
    cruise_speed = 160;
    battery_capacity = 220;
    charging_time = 0.8;
    energy_use = 2.2;
    faults_per_hour_prob = 0.05;
    number_passengers = 3;
    current_battery = battery_capacity;
    type = AircraftType::Charlie;
}

DeltaAircraft::DeltaAircraft() {
    cruise_speed = 90;
    battery_capacity = 220;
    charging_time = 0.8;
    energy_use = 2.2;
    faults_per_hour_prob = 0.22;
    number_passengers = 2;
    current_battery = battery_capacity;
    type = AircraftType::Delta;
}

EchoAircraft::EchoAircraft() {
    cruise_speed = 30;
    battery_capacity = 150;
    charging_time = 0.3;
    energy_use = 5.8;
    faults_per_hour_prob = 0.61;
    number_passengers = 2;
    current_battery = battery_capacity;
    type = AircraftType::Echo;
}

std::shared_ptr<Aircraft> AircraftFactory::create_aircraft(AircraftType type) {
    if (type == Alpha) {
        return std::make_shared<AlphaAircraft>();
    } else if (type == Bravo) {
        return std::make_shared<BravoAircraft>();
    } else if (type == Charlie) {
        return std::make_shared<CharlieAircraft>();
    } else if (type == Delta) {
        return std::make_shared<DeltaAircraft>();
    } else if (type == Echo) {
        return std::make_shared<EchoAircraft>();
    } else {
        return NULL;
    }
}
