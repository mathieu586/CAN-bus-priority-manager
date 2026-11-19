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
