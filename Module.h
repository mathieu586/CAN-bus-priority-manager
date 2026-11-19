#ifndef ZADANIE_2_MODULE_H
#define ZADANIE_2_MODULE_H
#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include "CANBus.h"
#include "CANFrame.h"

struct MsgType {
    std::string name;
    uint16_t id;
    int data_byte_size;
    int frequency_micro_s;
    int deadline_micro_s;
    int time_until_next_msg = 0;
};

class Module {
private:
    std::string name;
    std::vector<MsgType> msg_types;
public:
    Module(std::string name, std::vector<MsgType> msg_types);
    void update(CANBus& bus, unsigned long long current_time);
    CANFrame generate_frame(const MsgType& msgType, unsigned long long current_time);
};


#endif