#include "CANBus.h"
#include <ctime>
#include <format>
#include <iostream>

#include "Config.h"

using namespace std;

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
        }

        unsigned long long delay = current_time + time_until_free - current_frame.get_created_at();
        current_frame.set_delay(delay);
        msg_stats[current_frame.get_id()].total_delay += delay;
        if (delay > msg_stats[current_frame.get_id()].max_delay) {
            msg_stats[current_frame.get_id()].max_delay = delay;
        }

        msg_stats[current_frame.get_id()].total_msg_sent_count += 1;


        frame_queue.pop();
    }
}

int CANBus::get_late_msg_counter() const {
    return late_msg_counter;
}

int CANBus::get_queue_size() const {
    return frame_queue.size();
}

void CANBus::print_msg_stats() {
    cout << "================================ STATYSTKI WIADOMOSCI ================================\n";
    cout << "Liczba wiadomosci po terminie: " << late_msg_counter << "\n";
    for (const auto& [id, stat] : msg_stats) {
        double avg_delay = (stat.total_msg_sent_count != 0) ? stat.total_delay / stat.total_msg_sent_count : 0.0;
        cout << "\n*** Typ wiadomosci (id): " << hex << "0x" << id << dec << "***\n";
        cout << "Srednie opoznienie: " << std::format("{:.2f}\n", avg_delay);
        cout << "Maksymalne opoznienie: " << stat.max_delay << "\n";
    }
}
