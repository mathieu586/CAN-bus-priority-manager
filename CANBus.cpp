#include "CANBus.h"

#include <ctime>

#include "Config.h"

CANBus::CANBus(int speed) {
    this->speed = speed;
    time_until_free = 0;
    late_msg_counter = 0;
}

void CANBus::append_msg(const CANFrame& frame) {
    this->frame_queue.push(frame);
}

int CANBus::calculate_stuff_bits(int data_byte_size) {
    return (34 + 8 * data_byte_size - 1) / 4;
}

void CANBus::update(unsigned long long current_time) {
    if (time_until_free > 0) {
        time_until_free -= 1;
    }

    if (time_until_free <= 0 && !frame_queue.empty()) {
        CANFrame current_frame = frame_queue.top();
        time_until_free = current_frame.get_size() * TIME_TO_PROCESS_1_BIT;

        if (current_time + time_until_free > current_frame.get_deadline_at() && current_frame.get_deadline_at() != 0) {
            late_msg_counter += 1;
            current_frame.set_delay(current_time + time_until_free - current_frame.get_created_at());
        }

        frame_queue.pop();
    }
}

int CANBus::get_late_msg_counter() const {
    return late_msg_counter;
}

int CANBus::get_queue_size() const {
    return frame_queue.size();
}
