#include <memory>

enum AircraftType {
    Alpha, Bravo, Charlie, Delta, Echo, Count
};

enum AircraftState {
    Flying, Charging, Waiting, Count
};

class Aircraft {
public:
    // Aircraft characteristics
    double cruise_speed;           // mph
    double battery_capacity;       // kWh
    double charging_time;          // hours
    double energy_use;             // kWh/mile
    double faults_per_hour_prob;
    int passenger_count;
    AircraftType type;

    // Aircraft state
    AircraftState state;
    double current_battery;

    // Aircraft performance metrics
    int number_flights;
    int number_charges;
    int number_faults;
    double total_flight_time;           // seconds
    double total_charging_time;         // seconds
    double total_distance_travelled;    // miles

    Aircraft();

    void fly(double dt_seconds);

    void charge(double dt_seconds);

    int transition_state(int& number_aircraft_charging);
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
