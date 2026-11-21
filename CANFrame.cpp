#include "CANFrame.h"

CANFrame::CANFrame(int size, int id, unsigned long long created_at, unsigned long long deadline) {
    this->size = size;
    this->id = id;
    this->created_at = created_at;
    if (deadline > 0) {
        deadline_at = created_at + deadline;
    }else {
        deadline_at = 0;
    }
    delay = 0;
}

bool CANFrame::operator<(const CANFrame &other) const {
    if (this->id > other.id) {
        return true;
    }
    return false;
}

int CANFrame::get_size() const {
    return size;
}

int CANFrame::get_deadline_at() const {
    return deadline_at;
}

void CANFrame::set_delay(unsigned long long delay_val) {
    this->delay = delay_val;
}

unsigned long long CANFrame::get_created_at() {
    return created_at;
}
