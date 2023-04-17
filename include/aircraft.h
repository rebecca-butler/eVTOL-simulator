#pragma once

#include <memory>

class ChargingStation; // forward declaration

enum AircraftType {
    Alpha, Bravo, Charlie, Delta, Echo, Count
};

enum AircraftState {
    Flying, Charging, Waiting
};

struct Metrics {
    double avg_flight_time = 0;
    double avg_charging_time = 0;
    double avg_flight_distance = 0;
    double total_passenger_miles = 0;
    double total_faults = 0;
};

class Aircraft : public std::enable_shared_from_this<Aircraft> {
public:
    AircraftType get_type();

    bool is_battery_dead();

    bool is_battery_full();

    void fly(double dt);

    void charge(double dt);

    void update_state(ChargingStation& charging_station, double dt);

    Metrics compute_metrics();

    int compute_faults();

protected:
    Aircraft();

    // Aircraft characteristics
    double cruise_speed;           // mph
    double battery_capacity;       // kWh
    double charging_time;          // hours
    double energy_use;             // kWh/mile
    double faults_per_hour_prob;
    int number_passengers;
    AircraftType type;

    // Aircraft state
    AircraftState state;
    double current_battery;

    // Aircraft performance metrics
    int number_flights;
    int number_charges;
    int number_faults;
    double total_flight_time;           // hours
    double total_charging_time;         // hours
    double total_distance_travelled;    // miles
};
