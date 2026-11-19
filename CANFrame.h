#ifndef ZADANIE_2_CANFRAME_H
#define ZADANIE_2_CANFRAME_H
#include <cstdint>

class CANFrame {
private:
    int size;
    int id;
public:
    CANFrame(int size, int id);
    bool operator<(const CANFrame& other) const;
    int get_size() const;
};

#endif //ZADANIE_2_CANFRAME_H