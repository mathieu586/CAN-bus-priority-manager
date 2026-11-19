#include "Module.h"
#include "Config.h"
#include "CANBus.h"

using namespace std;

Module::Module(string name, vector<MsgType> msg_types) {
    this->name = name;
    this->msg_types = msg_types;
}

void Module::update(CANBus& bus, unsigned long long current_time) {
    for (MsgType& msgType : msg_types) {
        msgType.time_until_next_msg -= 1;
        if (msgType.time_until_next_msg <= 0) {
            bus.append_msg(generate_frame(msgType, current_time));

            msgType.time_until_next_msg = msgType.frequency_micro_s * 1000; // from ms to micro s
        }
    }
}

CANFrame Module::generate_frame(const MsgType& msgType, unsigned long long current_time) {
    CANFrame new_frame = CANFrame(msgType.data_byte_size * 8 + FRAME_OVERHEAD + FRAME_ISF + CANBus::calculate_stuff_bits(msgType.data_byte_size), msgType.id, current_time, msgType.deadline_micro_s);

    return new_frame;
}

