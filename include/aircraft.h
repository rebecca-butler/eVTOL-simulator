#pragma once

#include <memory>

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

// TODO: move to separate file
class Aircraft {
public:
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

    Aircraft();

    void fly(double dt_hours);

    void charge(double dt_hours);

    Metrics compute_metrics();

    int compute_faults();
};

class AlphaAircraft : public Aircraft {
public:
    AlphaAircraft();
};

class BravoAircraft : public Aircraft {
public:
    BravoAircraft();
};

class CharlieAircraft : public Aircraft {
public:
    CharlieAircraft();
};

class DeltaAircraft : public Aircraft {
public:
    DeltaAircraft();
};

class EchoAircraft : public Aircraft {
public:
    EchoAircraft();
};

class AircraftFactory {
public:
    static std::shared_ptr<Aircraft> create_aircraft(AircraftType type);
};
