cmake_minimum_required(VERSION 3.20)
project(can_bus_sim)

set(CMAKE_CXX_STANDARD 20)

add_executable(can_bus_sim main.cpp
        Module.cpp
        Module.h
        CanFrame.h
        CANFrame.cpp
        CANBus.h
        CANBus.cpp
        Config.h)
