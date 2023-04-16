#include <memory>

#include "aircraft.h"

Aircraft::Aircraft() {
    need_charge = false;
    is_charging = false;
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
