#pragma once

#include <memory>
#include <queue>

#include "aircraft.h"

class ChargingStation {
public:
    ChargingStation(int number_chargers_);

    void add_to_queue(std::shared_ptr<Aircraft> aircraft);

    void update();

private:
    int number_chargers;
    std::vector<std::shared_ptr<Aircraft>> charging_aircraft;
    std::queue<std::shared_ptr<Aircraft>> waiting_queue;
};
