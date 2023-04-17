#pragma once

#include <memory>
#include <queue>

#include "aircraft.h"

class ChargingStation {
public:
    ChargingStation(int number_chargers_);

    void add_to_charger();

    void remove_from_charger();

    void add_to_queue(std::shared_ptr<Aircraft> aircraft);

    void remove_from_queue();

    std::shared_ptr<Aircraft> get_next_in_queue();

    bool is_charger_available();

private:
    int number_chargers;
    int number_aircraft_charging;
    std::queue<std::shared_ptr<Aircraft>> queue;
};
