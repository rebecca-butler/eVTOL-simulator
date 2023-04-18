#include "aircraft.h"
#include "charging_station.h"

ChargingStation::ChargingStation(int number_chargers_) {
    number_chargers = number_chargers_;
    charging_aircraft.resize(number_chargers, nullptr);
}

void ChargingStation::add_to_queue(std::shared_ptr<Aircraft> aircraft) {
    waiting_queue.push(aircraft);
}

void ChargingStation::update() {
    // Update the state of charging aircraft and remove any that are finished charging
    for (int i = 0; i < number_chargers; i++) {
        std::shared_ptr<Aircraft> aircraft = charging_aircraft[i];
        if (aircraft != nullptr && aircraft->get_state() == AircraftState::Flying) {
            charging_aircraft[i] = nullptr;
        }
    }

    // Add waiting aircraft to available chargers
    for (int i = 0; i < number_chargers; i++) {
        if (charging_aircraft[i] == nullptr && !waiting_queue.empty()) {
            charging_aircraft[i] = waiting_queue.front();
            charging_aircraft[i]->set_state(AircraftState::Charging);
            waiting_queue.pop();
        }
    }
}