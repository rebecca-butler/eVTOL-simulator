cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 20)

project(evtol_simulator)

# Add gtest lib directory
add_subdirectory(gtest_lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Create simulator library using src files
add_library(simulator_lib 
    src/aircraft.cpp
    src/aircraft_factory.cpp
    src/charging_station.cpp
    src/simulator.cpp
)

# Set include directory for simulator library
target_include_directories(simulator_lib PUBLIC include)

# Create executables
add_executable(simulator src/simulator.cpp)
add_executable(test_aircraft test/test_aircraft.cpp)

# Link executables to libraries
target_link_libraries(test_aircraft PRIVATE simulator_lib gtest gtest_main)
target_link_libraries(simulator PRIVATE simulator_lib)
