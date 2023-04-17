#pragma once

#include <memory>

#include "aircraft.h"

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
