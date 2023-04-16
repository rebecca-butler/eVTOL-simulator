#include <iostream>
#include <memory>
#include <vector>

#include "aircraft.h"

int main() {
    int number_vehicles = 20;
    std::vector<std::shared_ptr<Aircraft>> vehicles;

    for (int i = 0; i < number_vehicles; i++) {
        // Randomly select an aircraft type
        AircraftType type = static_cast<AircraftType>(rand() % AircraftType::Count);

        // Create and aircraft
        vehicles.push_back(AircraftFactory::create_aircraft(type));
    }

    for (const auto vehicle : vehicles) {
        std::cout << vehicle->type << std::endl;
    }

    return 0;
}
