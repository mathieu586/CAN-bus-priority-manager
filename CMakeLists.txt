cmake_minimum_required(VERSION 4.1)
project(Zadanie_2)

set(CMAKE_CXX_STANDARD 26)

add_executable(Zadanie_2 main.cpp
        Module.cpp
        Module.h
        CanFrame.h
        CANFrame.cpp
        CANBus.h
        CANBus.cpp
        Config.h)
