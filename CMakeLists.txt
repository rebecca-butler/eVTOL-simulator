cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

project(evtol_simulator)

# Create library using src files
add_library(simulator_lib 
    src/aircraft.cpp
    src/aircraft_factory.cpp
    src/charging_station.cpp
    src/simulator.cpp
)

# Set include directory for library
target_include_directories(simulator_lib PUBLIC include)

# Create executable
add_executable(simulator src/simulator.cpp)

# Link executable to library
target_link_libraries(simulator PRIVATE simulator_lib)
