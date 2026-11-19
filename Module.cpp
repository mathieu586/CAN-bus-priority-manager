#include "Module.h"
#include "Config.h"
#include "CANBus.h"

using namespace std;

Module::Module(string name, vector<MsgType> msg_types) {
    this->name = name;
    this->msg_types = msg_types;
}

void Module::update(CANBus& bus) {
    for (MsgType& msgType : msg_types) {
        msgType.time_until_next_msg -= 1;
        if (msgType.time_until_next_msg <= 0) {
            bus.append_msg(generate_frame(msgType));

            msgType.time_until_next_msg = msgType.frequency_ms * 1000; // from ms to micro s
        }
    }
}

CANFrame Module::generate_frame(const MsgType& msgType) {
    CANFrame new_frame = CANFrame(msgType.data_byte_size * 8 + FRAME_OVERHEAD + FRAME_ISF + CANBus::calculate_stuff_bits(msgType.data_byte_size), msgType.id);

    return new_frame;
}

