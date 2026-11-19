#include "CANFrame.h"

CANFrame::CANFrame(int size, int id) {
    this->size = size;
    this->id = id;
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
