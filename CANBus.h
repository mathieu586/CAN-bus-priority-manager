#ifndef ZADANIE_2_CANBUS_H
#define ZADANIE_2_CANBUS_H
#pragma once
#include <queue>
#include "CANFrame.h"

struct msg_stats {

};

class CANBus {
private:
    int speed; // unit: bit/s
    int time_until_free;
    std::priority_queue<CANFrame> frame_queue;
    int late_msg_counter;
public:
    CANBus(int speed);
    void append_msg(const CANFrame& frame);
    static int calculate_stuff_bits(int data_byte_size);
    void update(unsigned long long current_time);
    int get_late_msg_counter() const;

    // temporary
    int get_queue_size() const;
};

#endif //ZADANIE_2_CANBUS_H