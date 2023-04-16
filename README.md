# eVTOL-simulator
This repo contains a simulation of 5 types of eVTOL aircraft. 

## Problem Statement

## Usage
Note: this project was developed and tested in a Windows 10 MINGW64 development environment. First, set up the project:

`mkdir build && cd build`
`cmake ..`

Then build:

`cmake --build .`

Then run the executable:

`./simulator`

## Assumptions
- The vehicles instantaneously arrive at the charging station when they need to charge
- The vehicles must be fully charged before they can resume flight
- The vehicles were designed with sufficient fault-tolerant measures and fail-safes such that when faults occur, they do not stop the vehicle from operating
- The vehicles are always carrying the maximum number of passengers

## Design

## Testing

## Simulation Results

## Future Considerations
- linting
- testing
- visualization
- better scenario configuration (config files)
- multithreading
- docstrings
- state transition diagram
- getters/setters
