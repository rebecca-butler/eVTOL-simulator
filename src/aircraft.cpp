#include <string>

enum AircraftType {
    Alpha, Bravo, Charlie, Delta, Echo
};

class Aircraft {
public:
    double cruise_speed;           // mph
    double battery_capacity;       // kWh
    double charging_time;          // hours
    double energy_use;             // kWh/mile
    double faults_per_hour_prob;
    int passenger_count;

    double current_battery;
    bool need_charge;
    bool is_charging;

    Aircraft() {
        need_charge = false;
        is_charging = false;
    }

    // Factory method
    static Aircraft* make_aircraft(AircraftType type);
};

class AlphaAircraft : public Aircraft {
public:
    AlphaAircraft() : Aircraft() {
        cruise_speed = 120;
        battery_capacity = 320;
        charging_time = 0.6;
        energy_use = 1.6;
        faults_per_hour_prob = 0.25;
        passenger_count = 4;
        current_battery = battery_capacity;
    }
};

class BravoAircraft : public Aircraft {
public:
    BravoAircraft() : Aircraft() {
        cruise_speed = 100;
        battery_capacity = 100;
        charging_time = 0.2;
        energy_use = 1.5;
        faults_per_hour_prob = 0.1;
        passenger_count = 5;
        current_battery = battery_capacity;
    }
};

static Aircraft* make_aircraft(AircraftType type) {
    if (type == Alpha) {
        return new AlphaAircraft();
    } else if (type == Bravo) {
        return new BravoAircraft();
    }
}