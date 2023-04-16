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
- The vehicles can instantaneously change between flying, charging, and waiting states
- When charging, the vehicles must be fully charged before they can resume flight
- Faults can only occur when the vehicles are flying, not when they are or charging or waiting
- When faults occur, there are sufficient fault-tolerant measures and fail-safes such that vehicles do not stop operating
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
