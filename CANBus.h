#ifndef ZADANIE_2_CANBUS_H
#define ZADANIE_2_CANBUS_H
#pragma once
#include <queue>
#include "CANFrame.h"
#include <map>

struct MsgStats {
    int total_msg_sent_count = 0;
    int total_delay = 0;
    int max_delay = 0;
};

class CANBus {
private:
    int speed; // unit: bit/s
    int time_until_free;
    std::priority_queue<CANFrame> frame_queue;
    int late_msg_counter;
    std::map<int, MsgStats> msg_stats;
public:
    CANBus(int speed);
    void append_msg(const CANFrame& frame);
    static int calculate_stuff_bits(int data_byte_size);
    void update(unsigned long long current_time);
    int get_queue_size() const;
    int get_late_msg_counter() const;
    void print_msg_stats();
};

#endif //ZADANIE_2_CANBUS_H