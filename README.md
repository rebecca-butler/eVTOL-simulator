# eVTOL-simulator
This repo contains a simulation of 5 types of eVTOL aircraft, each produced by a different manufacturer. The aircraft have unique characteristics that define their speed, battery capacity, and energy usage. The simulation runs for 3 hours with a total of 20 vehicles and 3 chargers available. When the simulation is complete, various metrics are reported per vehicle type, such as average flight time, average charging time, and number of passenger miles.

## System Requirements
- CMake >= 3.5.0
- GTest >= 1.13.0
- g++ (or other C++ 20 compiler)

Note: this project was developed and tested in a Windows 10 development environment.

## Usage
First, set up the project:

`mkdir build && cd build`
`cmake ..`

Then build:

`cmake --build .`

This will generate two executables. To run the simulation:

`./simulator`

To run the unit tests:

`./test_aircraft`

## Assumptions
- All vehicles start the simulation in the flying state
- The vehicles can instantaneously change between the flying, charging, and waiting states
- When charging, a vehicle must be fully charged before it can resume flight
- Faults can only occur when the vehicles are flying, not when they are or charging or waiting
- When faults occur, there are sufficient fault-tolerant measures and fail-safes such that the vehicles do not stop operating
- The vehicles are always carrying the maximum number of passengers

## Design
The simulation is based on three main classes: Aircraft, AircraftFactory, and ChargingStation. The Aircraft class is the base class for all types of eVTOL aircraft, and it includes functions to simulate flying and charging. These functions modify the current battery level and distance travelled by the aircraft. The AircraftFactory class generates instances of different types of aircraft based on the characteristics of each manufacturer.

The Aircraft class has four possible states: Flying, Charging, OutOfBattery, and InQueue. The `update_state()` function changes the aircraft's state based on its current status and the events happening in the simulation. For example, when an aircraft runs out of battery, it transitions from the Flying state to the OutOfBattery state and enters the queue to wait to charge.

The ChargingStation class is responsible for managing aircraft that are waiting to charge or currently charging. When a charger becomes available, aircraft in the waiting queue are moved to the Charging state. 

Finally, the simulator file handles the main loop of the simulation. This includes updating the state of each aircraft every second and reporting metrics at the end of the simulation.

Overall, the simulator was designed to be modular and easy to test and extend by following the OOP principles of inheritance and encapsulation.

## Testing
A small set of unit tests are provided using the GTest framework. The tests are focused on the Aircraft class, including the flying, charging, and state transition capabilities. The entire simulation was also run to verify that the metrics and behavior matched what was expected.

## Simulation Results
Sample output from one run of the simulation is presented below. The first table shows metrics for each aircraft individually, and the second shows total metrics for each type.

           Name    Num Flights   Avg Flight Time   Avg Flight Distance    Num Charges   Avg Charge Time    Faults   Passenger Miles
        Alpha 0              1              1:40                 200.0              1              0:10         1             800.0
        Alpha 1              1              1:40                 200.0              0              0:00         1             800.0
        Alpha 2              1              1:40                 200.0              0              0:00         1             800.0
        Alpha 3              1              1:40                 200.0              0              0:00         1             800.0
        Bravo 0              2              0:40                  66.7              1              0:12         0             666.7
        Bravo 1              2              0:40                  66.7              1              0:12         0             666.7
        Bravo 2              2              0:40                  66.7              1              0:12         0             666.7
        Bravo 3              2              0:40                  66.7              1              0:12         0             666.7
        Bravo 4              2              0:37                  62.0              1              0:12         0             620.3
      Charlie 0              2              0:37                 100.0              1              0:48         0             600.3
      Charlie 1              2              0:37                 100.0              1              0:48         0             600.3
      Charlie 2              2              0:37                 100.0              1              0:48         0             600.3
      Charlie 3              2              0:37                 100.0              1              0:48         0             600.3
      Charlie 4              2              0:37                 100.0              1              0:48         0             600.3
        Delta 0              1              1:06                 100.0              1              0:28         0             200.0
        Delta 1              1              1:06                 100.0              1              0:16         0             200.0
         Echo 0              2              0:40                  20.0              1              0:18         0              80.2
         Echo 1              2              0:40                  20.0              1              0:18         1              80.1
         Echo 2              2              0:34                  17.0              1              0:18         3              68.1
         Echo 3              2              0:31                  15.5              1              0:18         2              62.1

           Name    Num Flights   Avg Flight Time   Avg Flight Distance    Num Charges   Avg Charge Time    Faults   Passenger Miles
    Alpha Total              4              1:40                 200.0              1              0:02         4            3200.0
    Bravo Total             10              0:39                  65.7              5              0:12         0            3286.9
  Charlie Total             10              0:37                 100.0              5              0:48         0            3001.3
    Delta Total              2              1:06                 100.0              2              0:22         0             400.0
     Echo Total              8              0:36                  18.2              4              0:18         6             290.6


## Future Considerations
If this code were to be used commercially, a number of improvements could be made:

- Create a Dockerfile to allow for deployment in different environments
- Provide visualizations/graphs to better understand what happened to each aircraft
- Improve scenario configuration by allowing paramters to be defined in a config file
- Add additional unit tests for the AircraftFactory and ChargingStation classes to improve test coverage
- Add assertions to verify that parameters are valid (number of chargers cannot be negative, etc.)
- Consider a multi-threading approach to run in pseudo-realtime rather than updating states every second
- Add doxygen-style comments to improve code documentation
- Follow a company-specific linter or style guide to maintain consistency
