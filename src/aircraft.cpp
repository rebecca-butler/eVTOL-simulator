#include <memory>

#include "aircraft.h"

Aircraft::Aircraft() {
    // TODO: add other attributes
    state = AircraftState::Flying;
}

void Aircraft::fly(double dt_seconds) {
    // Compute distance travelled over the timestep
    // Units: (miles) = (miles/hr) * (sec) * (1 hr / 3600 sec)
    double distance_miles = cruise_speed * dt_seconds * 1/3600;

    // Update metrics
    total_flight_time++;
    total_distance_travelled += distance_miles;

    // Decrease battery level
    // Units: (kWh) = (kWh/mile) * (miles)
    current_battery -= energy_use * distance_miles;
}

void Aircraft::charge(double dt_seconds) {
    // Increase battery level
    // Units: (kWh/sec) = (kWh) / (hours) * (1 hr / 3600 sec) 
    current_battery += battery_capacity / charging_time * 1/3600;

    // Update metrics
    total_charging_time++;
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

AlphaAircraft::AlphaAircraft() {
    cruise_speed = 120;
    battery_capacity = 320;
    charging_time = 0.6;
    energy_use = 1.6;
    faults_per_hour_prob = 0.25;
    passenger_count = 4;
    current_battery = battery_capacity;
    type = AircraftType::Alpha;
}

BravoAircraft::BravoAircraft() {
    cruise_speed = 100;
    battery_capacity = 100;
    charging_time = 0.2;
    energy_use = 1.5;
    faults_per_hour_prob = 0.1;
    passenger_count = 5;
    current_battery = battery_capacity;
    type = AircraftType::Bravo;
}

CharlieAircraft::CharlieAircraft() {
    cruise_speed = 160;
    battery_capacity = 220;
    charging_time = 0.8;
    energy_use = 2.2;
    faults_per_hour_prob = 0.05;
    passenger_count = 3;
    current_battery = battery_capacity;
    type = AircraftType::Charlie;
}

DeltaAircraft::DeltaAircraft() {
    cruise_speed = 90;
    battery_capacity = 220;
    charging_time = 0.8;
    energy_use = 2.2;
    faults_per_hour_prob = 0.22;
    passenger_count = 2;
    current_battery = battery_capacity;
    type = AircraftType::Delta;
}

EchoAircraft::EchoAircraft() {
    cruise_speed = 30;
    battery_capacity = 150;
    charging_time = 0.3;
    energy_use = 5.8;
    faults_per_hour_prob = 0.61;
    passenger_count = 2;
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
