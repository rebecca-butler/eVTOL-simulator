#include <assert.h>

#include "aircraft.h"


#include "charging_station.h"

ChargingStation::ChargingStation(int number_chargers_) {
    number_chargers = number_chargers_;
    number_aircraft_charging = 0;
}

void ChargingStation::add_to_charger() {
    assert(is_charger_available());
    number_aircraft_charging++;
}

void ChargingStation::remove_from_charger() {
    assert(number_aircraft_charging > 0);
    number_aircraft_charging--;
}

void ChargingStation::add_to_queue(std::shared_ptr<Aircraft> aircraft) {
    queue.push(aircraft);
}

void ChargingStation::remove_from_queue() {
    queue.pop();
}

std::shared_ptr<Aircraft> ChargingStation::get_next_in_queue() {
    return queue.front();
}

bool ChargingStation::is_charger_available() {
    return number_aircraft_charging < number_chargers;
}
