#include <memory>

enum AircraftType {
    Alpha, Bravo, Charlie, Delta, Echo, Count
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
    AircraftType type;

    Aircraft();
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
