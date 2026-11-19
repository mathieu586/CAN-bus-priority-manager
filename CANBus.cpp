#include "CANBus.h"

CANBus::CANBus(int speed) {
    this->speed = speed;
    time_until_free = 0;
}

void CANBus::append_msg(const CANFrame& frame) {
    this->frame_queue.push(frame);
}

int CANBus::calculate_stuff_bits(int data_byte_size) {
    return (34 + 8 * data_byte_size - 1) / 4;
}
